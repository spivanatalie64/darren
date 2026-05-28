# YAcFS Architecture

## Overview

YAcFS is a **userspace filesystem** implemented via FUSE3 (libfuse3). All filesystem operations run in a userspace daemon process, with the kernel handling VFS integration and page caching.

```
┌─────────────────────────────────────────────────────┐
│                   Userspace                          │
│  ┌───────────────────────────────────────────────┐  │
│  │              YAcFS Daemon (yacfs)             │  │
│  │  ┌─────────┐ ┌──────────┐ ┌───────────────┐  │  │
│  │  │  FUSE   │ │ Metadata │ │  Block Cache  │  │  │
│  │  │  Ops    │ │ Manager  │ │  (4096 slots) │  │  │
│  │  └────┬────┘ └────┬─────┘ └───────┬───────┘  │  │
│  │       │           │               │          │  │
│  │  ┌────▼───────────▼───────────────▼───────┐  │  │
│  │  │          Pool Layer                    │  │  │
│  │  │  (blocks/ + meta/ + .snapshots/)       │  │  │
│  │  └──────────────────┬────────────────────┘  │  │
│  └─────────────────────┼───────────────────────┘  │
└────────────────────────┼──────────────────────────┘
                         │ FUSE device (/dev/fuse)
┌────────────────────────┼──────────────────────────┐
│  Kernel                │                          │
│  ┌─────────────────────▼──────────────────────┐   │
│  │            VFS Layer                       │   │
│  │  ┌──────────┐  ┌──────────────────────┐   │   │
│  │  │Page Cache│  │  FUSE Kernel Module  │   │   │
│  │  └──────────┘  └──────────────────────┘   │   │
│  └───────────────────────────────────────────┘   │
└──────────────────────────────────────────────────┘
```

## On-Disk Format

### Pool Directory Structure

```
<pool_root>/
├── blocks/           # Content-addressed block storage
│   ├── <hash>.blk   # Compressed + checksummed data blocks
│   └── ...
├── meta/             # Inode metadata
│   ├── <inode>.ino  # Per-inode metadata files
│   └── ...
└── .snapshots/       # Snapshot storage
    └── <name>/       # Snapshot directory (copy of meta/ at snapshot time)
```

### Block Format

Each block file in `blocks/` contains:

```
┌──────────────────────────────────────────────────────┐
│ struct block_hdr (20 bytes)                          │
├──────────────────┬───────────────────────────────────┤
│ magic (4 bytes)  │ POOL_MAGIC = 0x5A4653            │
│ checksum (4)     │ Lower 32 bits of xxhash3-64       │
│ orig_size (4)    │ Uncompressed data size            │
│ comp_size (4)    │ Stored (possibly compressed) size │
│ compress (1)     │ 0=none, 1=ZSTD                    │
│ csum_type (1)    │ 1=XXH64                           │
│ _pad (2)         │ Alignment padding                 │
├──────────────────┴───────────────────────────────────┤
│ Block data (comp_size bytes)                         │
│   - If compressed: ZSTD-compressed original data     │
│   - If uncompressed: raw data                        │
└──────────────────────────────────────────────────────┘
```

Blocks are named by the xxhash3-64 of their original uncompressed content. This provides **content-addressable storage** — identical content produces the same hash, which means the same block file is reused automatically.

### Inode Format

Each inode is stored as a separate file in `meta/`:

```c
struct yacfs_inode {
    uint64_t ino;           // Inode number
    uint64_t size;          // File size in bytes
    uint64_t mtime;         // Modification timestamp
    uint64_t ctime;         // Change timestamp
    uint32_t mode;          // File mode + type (S_IFREG, S_IFDIR, etc.)
    uint32_t uid;           // Owner user ID
    uint32_t gid;           // Owner group ID
    uint32_t nblocks;       // Number of blocks in the file
    uint32_t block_size;    // Block size (always 65536)
    uint32_t checksum_type; // Checksum algorithm (1=XXH64)
    uint32_t compress_type; // Compression algorithm (1=ZSTD)
};
// Followed by: uint64_t blocks[nblocks] — array of block hashes
```

### Directory Entry Format

Directory contents are stored as blocks containing packed directory entries:

```c
struct yacfs_dirent {
    uint16_t name_len;  // Length of entry name
    uint64_t ino;       // Target inode number
    uint8_t  type;      // DT_REG or DT_DIR
    char     name[];    // Entry name (variable length)
} __attribute__((packed));
```

## Caching Architecture

YAcFS uses three layers of caching:

### 1. Kernel Page Cache (Layer 1)

Enabled via FUSE `kernel_cache` option. The kernel caches file data in its page cache. Reads that hit the page cache never enter the YAcFS daemon — they return directly from kernel memory. This gives YAcFS the same read performance as any kernel filesystem on cached data.

### 2. Kernel Dentry/Attr Cache (Layer 2)

Configured with `attr_timeout=5.0` and `entry_timeout=5.0`. The kernel caches directory entries and file attributes for 5 seconds, avoiding FUSE round-trips for repeated `stat()` and `lookup()` calls.

### 3. Block Cache (Layer 3, in-daemon)

A 4096-slot LRU cache in the YAcFS daemon stores decompressed block data. When a read requires block data:

1. Check block cache (avoids decompression + disk read)
2. If miss: read from pool, decompress, verify checksum, insert into cache
3. Return data to caller

## Writeback Cache

YAcFS enables FUSE writeback cache (`FUSE_CAP_WRITEBACK_CACHE`). This allows the kernel to:

1. Batch small writes into larger ones
2. Return from `write()` immediately (before data hits the pool)
3. Flush data asynchronously in the background
4. Merge overlapping writes

The result is dramatically improved write throughput, especially for small random writes.

## Snapshots

Snapshots are implemented as metadata-level COW:

1. **Create**: Copy all inode files from `meta/` to `.snapshots/<name>/`
2. **List**: Read directory entries from `.snapshots/`
3. **Rollback**: Swap the `meta/` directory with `.snapshots/<name>/` (atomic rename)
4. **Current meta becomes a snapshot** (preserved for rollback recovery)

Since block files are content-addressed and immutable, snapshots share all unchanged blocks with the live filesystem (no data copying).
