# Contributing to YAcFS

YAcFS is developed under **AcreetionOS** by Natalie Spiva and Darren Clift. We welcome contributions from the community.

## Code of Conduct

Be excellent to each other. This is a project built by a trans woman and her father — we don't have time for bigotry, gatekeeping, or drama. If you wouldn't say it to someone's face, don't put it in a PR.

## Getting Started

1. Fork the repository
2. Clone your fork
3. Build YAcFS: `make -C docs/yacfs/`
4. Test: `mkdir /tmp/test && ./yacfs/yacfs /tmp/test -o pool=/tmp/test-pool`
5. Make your changes
6. Test your changes
7. Submit a pull request

## Development Setup

### Dependencies

```bash
# Arch Linux
sudo pacman -S fuse3 zstd xxhash base-devel

# Debian/Ubuntu
sudo apt install libfuse3-dev libzstd-dev libxxhash-dev build-essential

# Fedora
sudo dnf install fuse3-devel libzstd-devel xxhash-devel
```

### Build

```bash
make -C docs/yacfs/
```

### Run in Debug Mode

```bash
yacfs -f -d /mnt/test -o pool=/tmp/pool
```

This runs YAcFS in the foreground with full FUSE debug output.

## Code Style

- **C11** standard (GCC/Clang)
- **4-space indentation**, no tabs
- **K&R brace style** (opening brace on same line)
- **No trailing whitespace**
- **`__attribute__((packed))`** for on-disk structs
- **`_t` suffix** for typedefs (avoided — we use struct directly)
- **snake_case** for functions and variables
- **SCREAMING_SNAKE_CASE** for macros and constants
- Comments explain **why**, not **what** (the code should make *what* obvious)
- No unnecessary malloc: check return values always
- Always lock `st->lock` for write operations

### Example

```c
static int yacfs_do_something(struct yacfs_state *st, uint64_t param) {
    int ret = 0;
    void *buf = NULL;

    buf = malloc(1024);
    if (!buf) return -ENOMEM;

    pthread_mutex_lock(&st->lock);
    // ... do work ...
    pthread_mutex_unlock(&st->lock);

    free(buf);
    return ret;
}
```

## Testing

### Manual Testing

```bash
# Mount
yacfs /mnt/test -o pool=/tmp/pool

# Basic operations
echo "test" > /mnt/test/file.txt
cat /mnt/test/file.txt
mkdir /mnt/test/subdir
echo "nested" > /mnt/test/subdir/nested.txt

# Large files
dd if=/dev/urandom bs=1M count=100 of=/mnt/test/large.bin
dd if=/mnt/test/large.bin bs=1M count=100 of=/dev/null

# Verify checksums
md5sum /mnt/test/large.bin
md5sum /mnt/test/large.bin  # Should match

# Unmount
fusermount3 -u /mnt/test
```

### Stress Testing

```bash
# Run the stress test suite
python3 -c "
import os, random, string, hashlib

mnt = '/mnt/test'
os.system(f'yacfs {mnt} -o pool=/tmp/stress-pool')

# Create 1000 files with random content
for i in range(1000):
    data = ''.join(random.choices(string.ascii_letters, k=random.randint(1, 65536)))
    with open(f'{mnt}/f_{i}', 'w') as f:
        f.write(data)

# Verify every file
for i in range(1000):
    with open(f'{mnt}/f_{i}', 'r') as f:
        assert f.read(), f'File f_{i} failed'

print('Stress test passed')
os.system('fusermount3 -u ' + mnt)
"
```

## Pull Request Process

1. **One feature per PR** — Don't bundle unrelated changes
2. **Update docs** — If you change behavior, update the relevant docs
3. **Add tests** — Include test commands in the PR description
4. **No regressions** — Run the benchmark suite before submitting
5. **Sign your commits** — GPG or SSH signature preferred

## Reporting Bugs

Open an issue at https://github.com/spivanatalie64/spivanatalie64.github.io/issues with:

- YAcFS version (`yacfs --version` or build date)
- Kernel version (`uname -a`)
- FUSE version (`yacfs -h` or `pkg-config --modversion fuse3`)
- Steps to reproduce
- Any error messages from `dmesg` or the FUSE debug log

## Roadmap

### YAcFS v3 (Planned)

- **Kernel module** — Direct VFS integration (no FUSE overhead)
- **Fine-grained locking** — Per-inode locks instead of global mutex
- **RAID1 block mirroring** — Self-healing reads
- **Encryption** — Per-inode AES-256-GCM
- **Online fsck** — Background scrub with automatic repair
- **NFS/CIFS export** — Built-in network sharing
- **io_uring integration** — Zero-copy data path

### YAcFS v2 (Current)

- FUSE3 userspace daemon ✓
- Content-addressable block store ✓
- xxhash3 checksums ✓
- ZSTD compression (level 0-19) ✓
- Writeback cache ✓
- Kernel page + dentry cache ✓
- Block cache (4096-slot LRU) ✓
- COW metadata snapshots ✓
- Pool spanning (directory union) ✓

## Contact

- **Project Lead**: Natalie Spiva — natalie@acreetionos
- **Co-Developer**: Darren Clift — cobra3282000
- **IRC**: #acreetionos on libera.chat
- **GitHub**: https://github.com/spivanatalie64/spivanatalie64.github.io
