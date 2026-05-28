# YAcFS Developer API

## Source Tree

```
yacfs/
├── Makefile          # Build system
└── src/
    ├── yacfs.h       # Public header: structs, function declarations
    ├── main.c        # Entry point, option parsing, FUSE init
    ├── ops.c         # FUSE operations: getattr, readdir, read, write, create, etc.
    ├── pool.c        # Block pool: write, read, delete, compress, checksum, cache
    ├── metadata.c    # Inode metadata: save, load, delete, list
    └── snapshot.c    # Snapshots: create, list, rollback
```

## Key Data Structures

### `struct yacfs_state` (Global State)

```c
struct yacfs_state {
    struct pool_dir *pools;      // Array of pool directory paths
    int     npools;              // Number of pool directories
    char    meta_dir[4096];      // Path to metadata directory
    char    snap_dir[4096];      // Path to snapshots directory
    char    blk_dir[4096];       // Path to blocks directory
    uint64_t next_ino;           // Next available inode number
    int     compress_level;      // ZSTD compression level (0-19)
    pthread_mutex_t lock;        // Global state mutex
    struct cache_slot cache[4096]; // Decompressed block cache (LRU)
};
```

### `struct yacfs_inode` (On-Disk Inode)

```c
struct yacfs_inode {
    uint64_t ino;           // Unique inode identifier
    uint64_t size;          // File logical size (bytes)
    uint64_t mtime;         // Modification timestamp (unix seconds)
    uint64_t ctime;         // Change timestamp (unix seconds)
    uint32_t mode;          // File type + permissions (S_IFREG|0644, etc.)
    uint32_t uid;           // Owner POSIX UID
    uint32_t gid;           // Owner POSIX GID
    uint32_t nblocks;       // Number of blocks in file
    uint32_t block_size;    // Block size (always 65536)
    uint32_t checksum_type; // Checksum algorithm identifier
    uint32_t compress_type; // Compression algorithm identifier
};
// Followed by nblocks × uint64_t (block hashes)
```

### `struct block_hdr` (On-Disk Block Header)

```c
struct block_hdr {
    uint32_t magic;         // POOL_MAGIC (0x5A4653)
    uint32_t checksum;      // Lower 32 bits of xxhash3-64
    uint32_t orig_size;     // Original (uncompressed) data size
    uint32_t comp_size;     // Stored data size (may be compressed)
    uint8_t  compress;      // 0=none, 1=ZSTD
    uint8_t  checksum_type; // 1=XXH64
    uint16_t _pad;
} __attribute__((packed));
```

### `struct yacfs_dirent` (Directory Entry)

```c
struct yacfs_dirent {
    uint16_t name_len; // Length of entry name
    uint64_t ino;      // Target inode number
    uint8_t  type;     // DT_REG or DT_DIR
    char     name[];   // Entry name (variable length)
} __attribute__((packed));
```

## Core Functions

### Pool Layer (`pool.c`)

```c
// Hash data with xxhash3-64
uint64_t yacfs_hash(const void *data, size_t len);

// Compress data with ZSTD (level 1-19, 0=skip)
int yacfs_compress(int level, const void *src, size_t srclen, void *dst, size_t *dstlen);
int yacfs_decompress(const void *src, size_t srclen, void *dst, size_t dstlen);

// Write data to pool (dedup by hash, auto-compress for blocks ≥512 bytes)
int yacfs_pool_write(struct yacfs_state *st, const void *data, size_t len, uint64_t *hash_out);

// Read data from pool (checks cache first, verifies checksum on read)
int yacfs_pool_read(struct yacfs_state *st, uint64_t hash, void **data, size_t *len);

// Delete block file from pool
void yacfs_pool_delete(struct yacfs_state *st, uint64_t hash);
```

### Metadata Layer (`metadata.c`)

```c
// Save inode + block hash array to disk
int yacfs_meta_save(struct yacfs_state *st, struct yacfs_inode *inode, uint64_t *blocks);

// Load inode + block hash array from disk (caller must free both)
struct yacfs_inode *yacfs_meta_load(struct yacfs_state *st, uint64_t ino, uint64_t **blocks);

// Delete inode file
int yacfs_meta_delete(struct yacfs_state *st, uint64_t ino);

// List all inode numbers in the metadata directory
int yacfs_meta_list(struct yacfs_state *st, uint64_t **inos, int *count);
```

### Snapshots (`snapshot.c`)

```c
// Create snapshot: copy meta/ to .snapshots/<name>/
int yacfs_snap_create(struct yacfs_state *st, const char *name);

// List all snapshots in .snapshots/
int yacfs_snap_list(struct yacfs_state *st, char ***names, int *count);

// Rollback to snapshot: atomic rename swap
int yacfs_snap_rollback(struct yacfs_state *st, const char *name);

// Free snapshot list
void yacfs_snap_free_list(char ***names, int count);
```

### FUSE Operations (`ops.c`)

All standard FUSE operations are implemented. The operations table:

```c
const struct fuse_operations yacfs_ops = {
    .init     = yacfs_init,      // Enable writeback cache + kernel cache
    .getattr  = yacfs_getattr,    // stat()
    .readdir  = yacfs_readdir,    // readdir()
    .open     = yacfs_open,       // open()
    .read     = yacfs_read,       // read()
    .write    = yacfs_write,      // write() with writeback cache
    .create   = yacfs_create,     // creat() + open()
    .mkdir    = yacfs_mkdir,      // mkdir()
    .unlink   = yacfs_unlink,     // unlink()
    .rmdir    = yacfs_rmdir,      // rmdir()
    .truncate = yacfs_truncate,   // truncate()
    .utimens  = yacfs_utimens,    // utimensat()
    .chmod    = yacfs_chmod,      // chmod()
    .chown    = yacfs_chown,      // chown()
    .statfs   = yacfs_statfs,     // statfs()
};
```

## Thread Safety

YAcFS uses a single `pthread_mutex_t` global lock. All operations that modify state (create, write, unlink) acquire this lock. Read operations (read, getattr, readdir) do not currently lock — they rely on atomic inode file reads.

This is a known limitation. A fine-grained per-inode locking scheme is planned for v3.

## Adding New Features

### Adding a New FUSE Operation

1. Declare the function in `ops.c` with the standard FUSE signature
2. Add it to the `yacfs_ops` struct
3. Use `get_state()` to access global state
4. Lock `st->lock` for write operations

### Adding a New Compression Algorithm

1. Add a new `COMPRESS_*` constant in `yacfs.h`
2. Add compression logic in `yacfs_pool_write()` in `pool.c`
3. Add decompression logic in `yacfs_pool_read()` in `pool.c`
4. Update `struct yacfs_inode` if new fields are needed

### Adding a New Checksum Algorithm

1. Add a new `CHECKSUM_*` constant in `yacfs.h`
2. Update the checksum verification in `yacfs_pool_read()` in `pool.c`
3. Update the checksum storage in `yacfs_pool_write()` in `pool.c`
