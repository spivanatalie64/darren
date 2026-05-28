#ifndef YAcFS_H
#define YAcFS_H

#include <stdint.h>
#include <pthread.h>
#include <sys/stat.h>

#define BLOCK_SIZE        65536
#define POOL_MAGIC        0x5A4653
#define MAX_NAME_LEN      255
#define BUF_SIZE          1048576
#define CACHE_SLOTS       4096
#define COMPRESS_MIN      512

#define CHECKSUM_XXH64   1
#define COMPRESS_NONE    0
#define COMPRESS_ZSTD    1

struct block_hdr {
    uint32_t magic;
    uint32_t checksum;
    uint32_t orig_size;
    uint32_t comp_size;
    uint8_t  compress;
    uint8_t  checksum_type;
    uint16_t _pad;
} __attribute__((packed));

struct yacfs_inode {
    uint64_t    ino;
    uint64_t    size;
    uint64_t    mtime;
    uint64_t    ctime;
    uint32_t    mode;
    uint32_t    uid;
    uint32_t    gid;
    uint32_t    nblocks;
    uint32_t    block_size;
    uint32_t    checksum_type;
    uint32_t    compress_type;
};

struct yacfs_dirent {
    uint16_t    name_len;
    uint64_t    ino;
    uint8_t     type;
    char        name[];
} __attribute__((packed));

struct pool_dir {
    char   path[4096];
    size_t path_len;
};

struct cache_slot {
    uint64_t hash;
    uint8_t *data;
    size_t   len;
    uint64_t hits;
};

struct yacfs_state {
    struct pool_dir *pools;
    int     npools;
    char    meta_dir[4096];
    char    snap_dir[4096];
    char    blk_dir[4096];
    uint64_t next_ino;
    int     compress_level;
    pthread_mutex_t lock;
    struct cache_slot cache[CACHE_SLOTS];
};

uint64_t yacfs_hash(const void *data, size_t len);
int yacfs_compress(int level, const void *src, size_t srclen, void *dst, size_t *dstlen);
int yacfs_decompress(const void *src, size_t srclen, void *dst, size_t dstlen);
int yacfs_pool_write(struct yacfs_state *st, const void *data, size_t len, uint64_t *hash_out);
int yacfs_pool_read(struct yacfs_state *st, uint64_t hash, void **data, size_t *len);
void yacfs_pool_delete(struct yacfs_state *st, uint64_t hash);
int  yacfs_meta_save(struct yacfs_state *st, struct yacfs_inode *inode, uint64_t *blocks);
struct yacfs_inode *yacfs_meta_load(struct yacfs_state *st, uint64_t ino, uint64_t **blocks);
int  yacfs_meta_delete(struct yacfs_state *st, uint64_t ino);
int  yacfs_snap_create(struct yacfs_state *st, const char *name);
int  yacfs_snap_list(struct yacfs_state *st, char ***names, int *count);
int  yacfs_snap_rollback(struct yacfs_state *st, const char *name);
void yacfs_snap_free_list(char ***names, int count);
int  yacfs_meta_list(struct yacfs_state *st, uint64_t **inos, int *count);
extern const struct fuse_operations yacfs_ops;

#endif
