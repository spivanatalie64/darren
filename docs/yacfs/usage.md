# YAcFS Usage Guide

## Installation

### From Source

```bash
# Prerequisites
sudo pacman -S fuse3 zstd xxhash base-devel  # Arch
sudo apt install libfuse3-dev libzstd-dev libxxhash-dev  # Debian/Ubuntu

# Build
git clone https://github.com/spivanatalie64/spivanatalie64.github.io
cd spivanatalie64.github.io
make -C docs/yacfs
sudo cp docs/yacfs/yacfs /usr/local/bin/
```

### Pre-built Binary

Download from the [releases](https://github.com/spivanatalie64/spivanatalie64.github.io/releases) page.

## Basic Usage

### Create a Pool and Mount

```bash
yacfs /mnt/yacfs -o pool=/data/pool
```

This creates the following structure in `/data/pool`:
```
/data/pool/
├── blocks/        # Block storage (created automatically)
├── meta/          # Metadata (created automatically)
└── .snapshots/    # Snapshots (created automatically)
```

### Multiple Pool Directories

Span the filesystem across multiple directories (or disks):

```bash
yacfs /mnt/yacfs -o pool=/data/disk1:/data/disk2:/data/disk3
```

Blocks are searched across all pool directories. New blocks are written to the first pool.

### Compression

```bash
# Fast compression (default, recommended for most workloads)
yacfs /mnt/yacfs -o pool=/data/pool,compress=1

# Maximum compression (slower writes, better ratio)
yacfs /mnt/yacfs -o pool=/data/pool,compress=19

# No compression
yacfs /mnt/yacfs -o pool=/data/pool,compress=0
```

Compression level 1 (ZSTD) is the default. It offers the best speed/ratio tradeoff for most data. Blocks smaller than 512 bytes are never compressed (CPU overhead isn't worth it).

### Custom Snapshot Directory

```bash
yacfs /mnt/yacfs -o pool=/data/pool,snapdir=/data/snapshots
```

## Snapshots

### Create a Snapshot

While YAcFS is mounted, the snapshot API is called via the yacfs CLI (when unmounted management tools are added):

```bash
# The snapshot directory contains timestamped snapshots
ls /data/pool/.snapshots/
```

### List Snapshots

```bash
ls -la /data/pool/.snapshots/
```

### Rollback to a Snapshot

⚠ **WARNING**: Rollback replaces the current state with the snapshot state. Changes made after the snapshot will be lost.

```bash
# Stop YAcFS first
fusermount3 -u /mnt/yacfs

# Swap meta directory with snapshot
# (Implemented as atomic directory rename)
```

### Snapshot Internals

Snapshots copy inode metadata files. Block data is shared because blocks are content-addressed (named by their xxhash3-64 hash). A rollback restores the metadata to the snapshot state, and since all block hashes are still valid in the pool, all data is immediately accessible.

## Unmounting

```bash
fusermount3 -u /mnt/yacfs
```

## Performance Tuning

### Default Settings

| Parameter | Default | Description |
|---|---|---|
| `compress` | 1 | ZSTD compression level |
| `attr_timeout` | 5.0s | Kernel attribute cache TTL |
| `entry_timeout` | 5.0s | Kernel dentry cache TTL |
| Writeback cache | Enabled | Batch small writes |
| Kernel cache | Enabled | Use kernel page cache |
| Block cache | 4096 slots | LRU decompressed block cache |

### Workload-Specific Tuning

**Compressible data (logs, text, VMs):**
- Use `compress=3` for better ratio
- Write speed will be ~10-20% slower but storage use drops 2-5x

**Incompressible data (media, encrypted files, archives):**
- Use `compress=0` to skip compression entirely
- Avoids wasting CPU cycles trying to compress random data

**Mixed workload:**
- Default `compress=1` is optimal
- Blocks under 512 bytes skip compression automatically

## Pool Maintenance

### Garbage Collection (yacfs-gc)

Remove orphaned block files not referenced by any inode:

```bash
yacfs-gc /data/pool
```

Run this periodically to reclaim space after file deletions.

### Data Scrubbing (yacfs-scrub)

Proactively verify all data integrity:

```bash
yacfs-scrub /mnt/yacfs
```

Scrub reads every byte through the FUSE mount, triggering YAcFS's on-read checksum verification. Corrupted blocks produce `-EIO` errors and are reported with filename and path.

### Snapshots (yacfs-ctl)

```bash
# List snapshots
yacfs-ctl /data/pool snapshots

# Create a snapshot
yacfs-ctl /data/pool snapshot before-update

# Rollback to a snapshot
yacfs-ctl /data/pool rollback before-update
```

### Snapshot Replication (yacfs-replicate)

Send and receive snapshots between machines over TCP:

```bash
# On backup server
yacfs-receive /data/backup-pool 9999

# On production machine
yacfs-ctl /data/pool snapshot daily-backup
yacfs-send /data/pool daily-backup backup-server 9999
```

## Limitations

| Limitation | Status | Workaround |
|---|---|---|
| FUSE overhead on metadata | ~5-50µs per op | Kernel module planned (YAcFS v3) |
| No self-healing | Requires pool redundancy | Use RAID underneath |
| No online resize | Pool size fixed at creation | Pre-allocate large pool |
| No encryption | Planned for v3 | Use LUKS underneath |
