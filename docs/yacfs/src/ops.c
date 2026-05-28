#define FUSE_USE_VERSION 31
#include "yacfs.h"
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>

static struct yacfs_state *get_state(void) {
    return fuse_get_context()->private_data;
}

static int lookup_inode(struct yacfs_state *st, const char *path, uint64_t *ino) {
    if (strcmp(path, "/") == 0) { *ino = 1; return 0; }

    char *pathdup = strdup(path);
    char *save;
    char *comp = strtok_r(pathdup, "/", &save);

    uint64_t current = 1;
    int found = 1;

    while (comp && found) {
        found = 0;
        uint64_t *blks;
        struct yacfs_inode *dir_ino = yacfs_meta_load(st, current, &blks);
        if (!dir_ino || !S_ISDIR(dir_ino->mode)) {
            free(dir_ino); free(pathdup); return -ENOENT;
        }

        for (uint32_t i = 0; i < dir_ino->nblocks; i++) {
            void *data;
            size_t dlen;
            if (yacfs_pool_read(st, blks[i], &data, &dlen) < 0) continue;

            size_t off = 0;
            while (off + sizeof(uint16_t) <= dlen) {
                struct yacfs_dirent *e = (struct yacfs_dirent *)((uint8_t*)data + off);
                if (off + sizeof(uint16_t) + e->name_len > dlen) break;
                char ename[256];
                memcpy(ename, e->name, e->name_len);
                ename[e->name_len] = 0;
                if (strcmp(ename, comp) == 0) {
                    current = e->ino;
                    found = 1;
                    break;
                }
                off += sizeof(uint16_t) + e->name_len + sizeof(uint64_t) + sizeof(uint8_t);
            }
            free(data);
            if (found) break;
        }
        free(blks);
        free(dir_ino);
        comp = strtok_r(NULL, "/", &save);
    }

    free(pathdup);
    if (!found) return -ENOENT;
    *ino = current;
    return 0;
}

static int yacfs_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
    (void)fi;
    struct yacfs_state *st = get_state();
    memset(stbuf, 0, sizeof(*stbuf));

    uint64_t ino;
    int ret = lookup_inode(st, path, &ino);
    if (ret < 0) return ret;

    uint64_t *blocks;
    struct yacfs_inode *in = yacfs_meta_load(st, ino, &blocks);
    if (!in) return -ENOENT;

    stbuf->st_ino = in->ino;
    stbuf->st_mode = in->mode;
    stbuf->st_nlink = 1;
    stbuf->st_uid = in->uid;
    stbuf->st_gid = in->gid;
    stbuf->st_size = in->size;
    stbuf->st_blksize = BLOCK_SIZE;
    stbuf->st_blocks = (in->size + 511) / 512;
    stbuf->st_mtime = in->mtime;
    stbuf->st_ctime = in->ctime;
    stbuf->st_atime = time(NULL);

    free(blocks);
    free(in);
    return 0;
}

static int yacfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t off, struct fuse_file_info *fi, enum fuse_readdir_flags flags) {
    (void)off; (void)fi; (void)flags;
    struct yacfs_state *st = get_state();

    filler(buf, ".", NULL, 0, 0);
    filler(buf, "..", NULL, 0, 0);

    uint64_t ino;
    int ret = lookup_inode(st, path, &ino);
    if (ret < 0) return ret;

    uint64_t *blks;
    struct yacfs_inode *dir_in = yacfs_meta_load(st, ino, &blks);
    if (!dir_in || !S_ISDIR(dir_in->mode)) {
        free(blks); free(dir_in);
        return -ENOTDIR;
    }

    for (uint32_t i = 0; i < dir_in->nblocks; i++) {
        void *data;
        size_t dlen;
        if (yacfs_pool_read(st, blks[i], &data, &dlen) < 0) continue;
        size_t off2 = 0;
        while (off2 + sizeof(uint16_t) <= dlen) {
            struct yacfs_dirent *e = (struct yacfs_dirent *)((uint8_t*)data + off2);
            if (off2 + sizeof(uint16_t) + e->name_len > dlen) break;
            char ename[256];
            memcpy(ename, e->name, e->name_len);
            ename[e->name_len] = 0;
            struct stat sb = {0};
            uint64_t *b2;
            struct yacfs_inode *child = yacfs_meta_load(st, e->ino, &b2);
            if (child) {
                sb.st_mode = child->mode;
                sb.st_ino = child->ino;
                free(b2); free(child);
            }
            filler(buf, ename, &sb, 0, 0);
            off2 += sizeof(uint16_t) + e->name_len + sizeof(uint64_t) + sizeof(uint8_t);
        }
        free(data);
    }
    free(blks);
    free(dir_in);
    return 0;
}

static int yacfs_open(const char *path, struct fuse_file_info *fi) {
    return 0;
}

static int yacfs_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi) {
    (void)fi;
    struct yacfs_state *st = get_state();
    uint64_t ino;
    int ret = lookup_inode(st, path, &ino);
    if (ret < 0) return ret;

    uint64_t *blocks;
    struct yacfs_inode *in = yacfs_meta_load(st, ino, &blocks);
    if (!in) return -ENOENT;
    if (S_ISDIR(in->mode)) { free(blocks); free(in); return -EISDIR; }

    if (offset >= (off_t)in->size) { free(blocks); free(in); return 0; }
    if (offset + size > in->size) size = in->size - offset;

    size_t total = 0;
    while (total < size) {
        size_t blk_idx = (offset + total) / BLOCK_SIZE;
        size_t blk_off = (offset + total) % BLOCK_SIZE;
        if (blk_idx >= in->nblocks) break;

        void *bdata;
        size_t blen;
        if (yacfs_pool_read(st, blocks[blk_idx], &bdata, &blen) < 0) break;

        size_t to_copy = blen - blk_off;
        if (to_copy > size - total) to_copy = size - total;
        memcpy(buf + total, (uint8_t*)bdata + blk_off, to_copy);
        free(bdata);
        total += to_copy;
    }

    free(blocks);
    free(in);
    return total;
}

static int yacfs_write(const char *path, const char *buf, size_t size,
                       off_t offset, struct fuse_file_info *fi) {
    (void)fi;
    struct yacfs_state *st = get_state();

    pthread_mutex_lock(&st->lock);

    uint64_t ino;
    int ret = lookup_inode(st, path, &ino);
    if (ret < 0) { pthread_mutex_unlock(&st->lock); return ret; }

    uint64_t *blocks = NULL;
    struct yacfs_inode *in = yacfs_meta_load(st, ino, &blocks);
    if (!in) { pthread_mutex_unlock(&st->lock); return -ENOENT; }
    if (S_ISDIR(in->mode)) { free(blocks); free(in); pthread_mutex_unlock(&st->lock); return -EISDIR; }

    size_t end = offset + size;
    uint32_t needed = (end + BLOCK_SIZE - 1) / BLOCK_SIZE;

    if (needed > in->nblocks) {
        blocks = realloc(blocks, needed * sizeof(uint64_t));
        memset(blocks + in->nblocks, 0, (needed - in->nblocks) * sizeof(uint64_t));
    }

    size_t total = 0;
    while (total < size) {
        size_t blk_idx = (offset + total) / BLOCK_SIZE;
        size_t blk_off = (offset + total) % BLOCK_SIZE;

        if (blk_idx >= needed) break;

        void *bdata = NULL;
        size_t blen = BLOCK_SIZE;

        if (blocks[blk_idx] != 0) {
            if (yacfs_pool_read(st, blocks[blk_idx], &bdata, &blen) < 0) {
                bdata = calloc(1, BLOCK_SIZE);
                blen = BLOCK_SIZE;
            }
        } else {
            bdata = calloc(1, BLOCK_SIZE);
            blen = BLOCK_SIZE;
        }

        size_t to_write = BLOCK_SIZE - blk_off;
        if (to_write > size - total) to_write = size - total;

        memcpy((uint8_t*)bdata + blk_off, buf + total, to_write);

        uint64_t new_hash;
            yacfs_pool_write(st, bdata, blen, &new_hash);
        blocks[blk_idx] = new_hash;

        free(bdata);
        total += to_write;
    }

    in->nblocks = needed;
    if (end > in->size) in->size = end;
    in->mtime = time(NULL);
    in->ctime = in->mtime;

    yacfs_meta_save(st, in, blocks);
    free(blocks);
    free(in);
    pthread_mutex_unlock(&st->lock);
    return total;
}

static int yacfs_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    (void)fi;
    struct yacfs_state *st = get_state();
    pthread_mutex_lock(&st->lock);

    char *parent = strdup(path);
    char *name = strrchr(parent, '/');
    if (!name) { free(parent); pthread_mutex_unlock(&st->lock); return -EINVAL; }
    *name = 0;
    name++;
    if (*name == 0) { free(parent); pthread_mutex_unlock(&st->lock); return -EINVAL; }

    size_t namelen = strlen(name);
    uint64_t pino;
    int ret = lookup_inode(st, parent[0] ? parent : "/", &pino);
    if (ret < 0) { free(parent); pthread_mutex_unlock(&st->lock); return ret; }

    uint64_t *pblocks;
    struct yacfs_inode *pdir = yacfs_meta_load(st, pino, &pblocks);
    if (!pdir || !S_ISDIR(pdir->mode)) {
        free(pblocks); free(pdir); free(parent);
        pthread_mutex_unlock(&st->lock); return -ENOTDIR;
    }

    uint64_t new_ino = st->next_ino++;
    struct yacfs_inode new_in = {
        .ino = new_ino,
        .size = 0,
        .mtime = time(NULL),
        .ctime = time(NULL),
        .mode = mode,
        .uid = fuse_get_context()->uid,
        .gid = fuse_get_context()->gid,
        .nblocks = 0,
        .block_size = BLOCK_SIZE,
        .checksum_type = CHECKSUM_XXH64,
        .compress_type = COMPRESS_ZSTD,
    };
    yacfs_meta_save(st, &new_in, NULL);

    size_t entry_size = sizeof(uint16_t) + namelen + sizeof(uint64_t) + sizeof(uint8_t);
    void *entry = malloc(entry_size);
    struct yacfs_dirent *de = entry;
    de->name_len = namelen;
    de->ino = new_ino;
    de->type = S_ISDIR(mode) ? DT_DIR : DT_REG;
    memcpy(de->name, name, namelen);

    uint64_t ehash;
    yacfs_pool_write(st, entry, entry_size, &ehash);
    free(entry);

    size_t old_n = pdir->nblocks;
    pdir->nblocks = old_n + 1;
    uint64_t *new_pblocks = realloc(pblocks, pdir->nblocks * sizeof(uint64_t));
    new_pblocks[old_n] = ehash;
    yacfs_meta_save(st, pdir, new_pblocks);

    free(new_pblocks);
    free(pdir);
    free(parent);
    pthread_mutex_unlock(&st->lock);
    return 0;
}

static int yacfs_mkdir(const char *path, mode_t mode) {
    struct fuse_file_info fi = {0};
    return yacfs_create(path, S_IFDIR | (mode & 0777), &fi);
}

static int yacfs_unlink(const char *path) {
    struct yacfs_state *st = get_state();
    pthread_mutex_lock(&st->lock);

    char *parent = strdup(path);
    char *name = strrchr(parent, '/');
    if (!name) { free(parent); pthread_mutex_unlock(&st->lock); return -EINVAL; }
    *name = 0;
    name++;
    size_t unamelen = strlen(name);
    uint64_t pino;
    int ret = lookup_inode(st, parent[0] ? parent : "/", &pino);
    if (ret < 0) { free(parent); pthread_mutex_unlock(&st->lock); return ret; }

    uint64_t ino;
    ret = lookup_inode(st, path, &ino);
    if (ret < 0) { free(parent); pthread_mutex_unlock(&st->lock); return ret; }

    uint64_t *blks;
    struct yacfs_inode *in = yacfs_meta_load(st, ino, &blks);
    if (in) {
        for (uint32_t i = 0; i < in->nblocks; i++)
            yacfs_pool_delete(st, blks[i]);
        free(blks);
        yacfs_meta_delete(st, ino);
        free(in);
    }

    uint64_t *pblks;
    struct yacfs_inode *pdir = yacfs_meta_load(st, pino, &pblks);
    if (pdir && S_ISDIR(pdir->mode)) {
        int found = -1;
        size_t namelen = unamelen;
        for (uint32_t i = 0; i < pdir->nblocks; i++) {
            void *data;
            size_t dlen;
            if (yacfs_pool_read(st, pblks[i], &data, &dlen) < 0) continue;
            struct yacfs_dirent *e = (struct yacfs_dirent *)data;
            if (e->name_len == namelen && memcmp(e->name, name, namelen) == 0) {
                found = i;
                free(data);
                break;
            }
            free(data);
        }
        if (found >= 0) {
            yacfs_pool_delete(st, pblks[found]);
            for (int i = found; i < (int)pdir->nblocks - 1; i++)
                pblks[i] = pblks[i + 1];
            pdir->nblocks--;
            yacfs_meta_save(st, pdir, pblks);
        }
    }
    free(pblks);
    free(pdir);
    free(parent);
    pthread_mutex_unlock(&st->lock);
    return 0;
}

static int yacfs_rmdir(const char *path) {
    return yacfs_unlink(path);
}

static int yacfs_truncate(const char *path, off_t size, struct fuse_file_info *fi) {
    (void)fi;
    struct yacfs_state *st = get_state();
    pthread_mutex_lock(&st->lock);

    uint64_t ino;
    int ret = lookup_inode(st, path, &ino);
    if (ret < 0) { pthread_mutex_unlock(&st->lock); return ret; }

    uint64_t *blocks;
    struct yacfs_inode *in = yacfs_meta_load(st, ino, &blocks);
    if (!in) { pthread_mutex_unlock(&st->lock); return -ENOENT; }

    in->size = size;
    in->mtime = time(NULL);
    in->ctime = in->mtime;
    yacfs_meta_save(st, in, blocks);
    free(blocks);
    free(in);
    pthread_mutex_unlock(&st->lock);
    return 0;
}

static int yacfs_utimens(const char *path, const struct timespec tv[2],
                         struct fuse_file_info *fi) {
    (void)fi;
    struct yacfs_state *st = get_state();
    uint64_t ino;
    int ret = lookup_inode(st, path, &ino);
    if (ret < 0) return ret;

    uint64_t *blocks;
    struct yacfs_inode *in = yacfs_meta_load(st, ino, &blocks);
    if (!in) return -ENOENT;

    in->mtime = tv[1].tv_sec;
    in->ctime = time(NULL);
    yacfs_meta_save(st, in, blocks);
    free(blocks);
    free(in);
    return 0;
}

static int yacfs_chmod(const char *path, mode_t mode, struct fuse_file_info *fi) {
    (void)fi;
    struct yacfs_state *st = get_state();
    uint64_t ino;
    int ret = lookup_inode(st, path, &ino);
    if (ret < 0) return ret;

    uint64_t *blocks;
    struct yacfs_inode *in = yacfs_meta_load(st, ino, &blocks);
    if (!in) return -ENOENT;

    in->mode = (in->mode & S_IFMT) | (mode & 07777);
    in->ctime = time(NULL);
    yacfs_meta_save(st, in, blocks);
    free(blocks);
    free(in);
    return 0;
}

static int yacfs_chown(const char *path, uid_t uid, gid_t gid,
                       struct fuse_file_info *fi) {
    (void)fi;
    struct yacfs_state *st = get_state();
    uint64_t ino;
    int ret = lookup_inode(st, path, &ino);
    if (ret < 0) return ret;

    uint64_t *blocks;
    struct yacfs_inode *in = yacfs_meta_load(st, ino, &blocks);
    if (!in) return -ENOENT;

    if (uid != (uid_t)-1) in->uid = uid;
    if (gid != (gid_t)-1) in->gid = gid;
    in->ctime = time(NULL);
    yacfs_meta_save(st, in, blocks);
    free(blocks);
    free(in);
    return 0;
}

static void *yacfs_init(struct fuse_conn_info *conn, struct fuse_config *cfg) {
    conn->want |= FUSE_CAP_WRITEBACK_CACHE;
    conn->want |= FUSE_CAP_SPLICE_READ;
    conn->want |= FUSE_CAP_SPLICE_WRITE;
    cfg->kernel_cache = 1;
    cfg->attr_timeout = 5.0;
    cfg->entry_timeout = 5.0;
    cfg->negative_timeout = 1.0;
    return fuse_get_context()->private_data;
}

static int yacfs_statfs(const char *path, struct statvfs *stbuf) {
    (void)path;
    struct yacfs_state *st = get_state();
    memset(stbuf, 0, sizeof(*stbuf));
    stbuf->f_bsize = BLOCK_SIZE;
    stbuf->f_frsize = BLOCK_SIZE;
    stbuf->f_namemax = MAX_NAME_LEN;

    struct stat sb;
    if (stat(st->pools[0].path, &sb) == 0) {
        stbuf->f_blocks = sb.st_size / BLOCK_SIZE;
        stbuf->f_bfree = sb.st_size / BLOCK_SIZE;
        stbuf->f_bavail = sb.st_size / BLOCK_SIZE;
    }
    return 0;
}

const struct fuse_operations yacfs_ops = {
    .init     = yacfs_init,
    .getattr  = yacfs_getattr,
    .readdir  = yacfs_readdir,
    .open     = yacfs_open,
    .read     = yacfs_read,
    .write    = yacfs_write,
    .create   = yacfs_create,
    .mkdir    = yacfs_mkdir,
    .unlink   = yacfs_unlink,
    .rmdir    = yacfs_rmdir,
    .truncate = yacfs_truncate,
    .utimens  = yacfs_utimens,
    .chmod    = yacfs_chmod,
    .chown    = yacfs_chown,
    .statfs   = yacfs_statfs,
};
