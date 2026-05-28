# Sprungles.org — Home of AcreetionOS & YAcFS

**Natalie Spiva** — Lead Infrastructure Architect
**Darren Clift** — Co-Lead Developer

---

## Why YAcFS?

We built YAcFS because the existing filesystem landscape is broken.

**ZFS** is trapped behind a CDDL licensing wall that makes it impossible to ship as part of the Linux kernel. Every distro that wants ZFS has to jump through hoops — DKMS, out-of-tree modules, license incompatibility fights. It's 2026 and we're still dealing with this. ZFS is also a memory hog, requires tuning to run well on commodity hardware, and its RAID-Z expansion is still painful.

**btrfs** was supposed to be the answer. It's in the kernel, it's GPL, it has all the features. But after a decade and a half, it still has edge cases that eat your data — the RAID56 code is still considered dangerous, send/receive can produce corrupted streams under load, and performance on spinning rust is genuinely bad. We've lost data to btrfs. We're done trusting it.

**Everything else** (ext4, XFS) is a journaling filesystem from the 90s. No checksums. No snapshots. No compression. No self-healing. In 2026, storing data without integrity verification is irresponsible.

So we wrote YAcFS — **Yet Another common File System** — the filesystem that should have existed all along. 

YAcFS is:
- **GPLv3** — No licensing bullshit. Ships in every kernel. Every distro.
- **Fast** — Writeback cache, kernel page cache, block-level content dedup, xxhash3 checksums, ZSTD compression tuned for speed.
- **Simple** — One binary, one pool directory. Mount it and go.
- **Safe** — Checksums on every read. Content-addressable blocks. COW snapshots.
- **Pooled** — Union multiple directories into one filesystem pool.

We built it because we were tired of choosing between "works with Linux" and "has basic data integrity features." YAcFS gives you both — and outperforms both ZFS and btrfs on real-world workloads.

**Built by AcreetionOS — for everyone.**

---

## YAcFS Documentation

Full documentation is available under [`docs/yacfs/`](docs/yacfs/):

| Document | Description |
|---|---|
| [Overview](docs/yacfs/) | What YAcFS is and its key features |
| [Architecture](docs/yacfs/architecture.md) | On-disk format, block store, metadata, caching |
| [Usage](docs/yacfs/usage.md) | Installation, mounting, snapshots, pooling |
| [Performance](docs/yacfs/performance.md) | Benchmarks vs ZFS, btrfs, ext4 |
| [API](docs/yacfs/api.md) | Developer documentation, FUSE ops reference |
| [Contributing](docs/yacfs/contributing.md) | How to contribute to YAcFS |

---

*"99.9% uptime — not just in the systems I build, but in my own survival and evolution."* — Natalie Spiva
