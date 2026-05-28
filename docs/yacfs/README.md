# YAcFS — Yet Another common File System

**Version 2.0** — Copyright (c) 2026 AcreetionOS

YAcFS is a copy-on-write, content-addressed, checksummed userspace filesystem built on FUSE3. It provides ZFS-class data integrity features (checksums, compression, snapshots, pooling) without ZFS's licensing baggage or btrfs's stability track record.

## Quick Start

```bash
# Install
sudo cp yacfs /usr/local/bin/

# Create a pool and mount
mkdir -p /mnt/yacfs /data/pool
yacfs /mnt/yacfs -o pool=/data/pool

# Use it
echo "hello world" > /mnt/yacfs/test.txt
cat /mnt/yacfs/test.txt

# Unmount
fusermount3 -u /mnt/yacfs
```

## Key Features

| Feature | YAcFS | ZFS | btrfs | ext4 |
|---|---|---|---|---|
| **Checksums** | xxhash3 (every read) | fletcher4/SHA-256 | crc32c/xxhash | None |
| **Compression** | ZSTD (per-block) | lz4/zstd/gzip | zstd/lzo/zlib | None |
| **Snapshots** | COW metadata snapshots | Native | Native (subvol) | None |
| **Pool spanning** | Directory union | vdev pools | No | No |
| **Block dedup** | Content-addressed | DDT (slow) | No | No |
| **Kernel license** | GPLv3 | CDDL (incompatible) | GPLv2 | GPLv2 |
| **Writeback cache** | Yes | No (sync by default) | Yes | Yes |

## Data Integrity

Every block stored by YAcFS includes an xxhash3-64 checksum of the original (uncompressed) data. On every read, YAcFS:

1. Reads the block header from the pool
2. Decompresses (if compressed)
3. Recomputes the xxhash3-64 checksum
4. Compares against the stored checksum
5. Returns `-EIO` on mismatch — data never silently corrupts

Combined with content-addressable storage (blocks named by their hash), YAcFS also provides **free block-level deduplication** — identical data writes only consume storage once.

## Credits

YAcFS is developed under **AcreetionOS**:

- **Natalie Spiva** — Project Lead, Lead Architect
- **Darren Clift** — Co-Developer, Testing
- Built on: FUSE3, xxHash (Yann Collet), Zstandard (Facebook/Meta)

See [CONTRIBUTORS](CONTRIBUTORS) for the full list.
