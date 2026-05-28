#include "yacfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <xxhash.h>
#include <zstd.h>
#include <sys/stat.h>

uint64_t yacfs_hash(const void *data, size_t len) {
    return XXH3_64bits(data, len);
}

int yacfs_compress(int level, const void *src, size_t srclen, void *dst, size_t *dstlen) {
    size_t bound = ZSTD_compressBound(srclen);
    if (*dstlen < bound) return -1;
    size_t res = ZSTD_compress(dst, *dstlen, src, srclen, level);
    if (ZSTD_isError(res)) return -1;
    *dstlen = res;
    return 0;
}

int yacfs_decompress(const void *src, size_t srclen, void *dst, size_t dstlen) {
    size_t res = ZSTD_decompress(dst, dstlen, src, srclen);
    if (ZSTD_isError(res)) return -1;
    return 0;
}

static void hash_to_path(uint64_t hash, char *out, size_t outlen) {
    snprintf(out, outlen, "%016lx.blk", hash);
}

static int find_in_pool(struct yacfs_state *st, const char *subdir, const char *fname, char *fullpath, size_t fullsz) {
    for (int i = 0; i < st->npools; i++) {
        snprintf(fullpath, fullsz, "%s/%s/%s", st->pools[i].path, subdir, fname);
        struct stat sb;
        if (stat(fullpath, &sb) == 0)
            return 0;
        if (errno != ENOENT)
            return -1;
    }
    snprintf(fullpath, fullsz, "%s/%s/%s", st->pools[0].path, subdir, fname);
    return 0;
}

static void cache_insert(struct yacfs_state *st, uint64_t hash, const void *data, size_t len) {
    int slot = -1;
    uint64_t min_hits = UINT64_MAX;
    for (int i = 0; i < CACHE_SLOTS; i++) {
        if (!st->cache[i].data) { slot = i; break; }
        if (st->cache[i].hits < min_hits) {
            min_hits = st->cache[i].hits;
            slot = i;
        }
    }
    if (slot < 0) return;
    if (st->cache[slot].data) free(st->cache[slot].data);
    st->cache[slot].hash = hash;
    st->cache[slot].data = malloc(len);
    memcpy(st->cache[slot].data, data, len);
    st->cache[slot].len = len;
    st->cache[slot].hits = 1;
}

static int cache_lookup(struct yacfs_state *st, uint64_t hash, void **data, size_t *len) {
    for (int i = 0; i < CACHE_SLOTS; i++) {
        if (st->cache[i].data && st->cache[i].hash == hash) {
            st->cache[i].hits++;
            *data = malloc(st->cache[i].len);
            memcpy(*data, st->cache[i].data, st->cache[i].len);
            *len = st->cache[i].len;
            return 1;
        }
    }
    return 0;
}

int yacfs_pool_write(struct yacfs_state *st, const void *data, size_t len, uint64_t *hash_out) {
    uint64_t hash = yacfs_hash(data, len);
    *hash_out = hash;

    char fname[64], fpath[4096];
    hash_to_path(hash, fname, sizeof(fname));

    if (find_in_pool(st, "blocks", fname, fpath, sizeof(fpath)) == 0) {
        struct stat sb;
        if (stat(fpath, &sb) == 0)
            return 0;
    }

    int do_compress = (len >= COMPRESS_MIN) && st->compress_level > 0;
    size_t store_len = len;
    const void *store_data = data;
    void *comp_buf = NULL;

    if (do_compress) {
        size_t comp_bound = ZSTD_compressBound(len);
        comp_buf = malloc(comp_bound);
        if (comp_buf) {
            size_t comp_len = comp_bound;
            if (yacfs_compress(st->compress_level, data, len, comp_buf, &comp_len) == 0 && comp_len < len) {
                store_len = comp_len;
                store_data = comp_buf;
            } else {
                do_compress = 0;
            }
        } else {
            do_compress = 0;
        }
    }

    struct block_hdr hdr = {
        .magic = POOL_MAGIC,
        .checksum = (uint32_t)(hash & 0xFFFFFFFF),
        .orig_size = (uint32_t)len,
        .comp_size = (uint32_t)store_len,
        .compress = do_compress ? COMPRESS_ZSTD : COMPRESS_NONE,
        .checksum_type = CHECKSUM_XXH64,
    };

    uint8_t *buf = malloc(sizeof(hdr) + store_len);
    if (!buf) { free(comp_buf); return -ENOMEM; }
    memcpy(buf, &hdr, sizeof(hdr));
    memcpy(buf + sizeof(hdr), store_data, store_len);

    snprintf(fpath, sizeof(fpath), "%s/blocks/%s", st->pools[0].path, fname);
    int fd = open(fpath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { free(buf); free(comp_buf); return -errno; }
    size_t written = 0;
    while (written < sizeof(hdr) + store_len) {
        ssize_t n = write(fd, buf + written, sizeof(hdr) + store_len - written);
        if (n <= 0) break;
        written += n;
    }
    close(fd);
    free(buf);
    free(comp_buf);
    return (written == sizeof(hdr) + store_len) ? 0 : -EIO;
}

int yacfs_pool_read(struct yacfs_state *st, uint64_t hash, void **data, size_t *len) {
    if (cache_lookup(st, hash, data, len))
        return 0;

    char fname[64], fpath[4096];
    hash_to_path(hash, fname, sizeof(fname));

    if (find_in_pool(st, "blocks", fname, fpath, sizeof(fpath)) < 0)
        return -ENOENT;

    int fd = open(fpath, O_RDONLY);
    if (fd < 0) return -errno;

    struct block_hdr hdr;
    if (read(fd, &hdr, sizeof(hdr)) != sizeof(hdr)) { close(fd); return -EIO; }
    if (hdr.magic != POOL_MAGIC) { close(fd); return -EIO; }

    size_t store_len = hdr.comp_size;
    void *store_buf = malloc(store_len);
    if (!store_buf) { close(fd); return -ENOMEM; }

    size_t total = 0;
    while (total < store_len) {
        ssize_t n = read(fd, (uint8_t*)store_buf + total, store_len - total);
        if (n <= 0) break;
        total += n;
    }
    close(fd);
    if (total != store_len) { free(store_buf); return -EIO; }

    size_t orig_size = hdr.orig_size;
    void *orig_buf = malloc(orig_size);
    if (!orig_buf) { free(store_buf); return -ENOMEM; }

    if (hdr.compress == COMPRESS_ZSTD) {
        if (yacfs_decompress(store_buf, store_len, orig_buf, orig_size) < 0) {
            free(store_buf); free(orig_buf); return -EIO;
        }
    } else {
        memcpy(orig_buf, store_buf, store_len);
    }
    free(store_buf);

    uint64_t check = yacfs_hash(orig_buf, orig_size);
    if ((uint32_t)(check & 0xFFFFFFFF) != hdr.checksum) {
        free(orig_buf);
        return -EIO;
    }

    cache_insert(st, hash, orig_buf, orig_size);
    *data = orig_buf;
    *len = orig_size;
    return 0;
}

void yacfs_pool_delete(struct yacfs_state *st, uint64_t hash) {
    char fname[64], fpath[4096];
    hash_to_path(hash, fname, sizeof(fname));
    for (int i = 0; i < st->npools; i++) {
        snprintf(fpath, sizeof(fpath), "%s/blocks/%s", st->pools[i].path, fname);
        unlink(fpath);
    }
}
