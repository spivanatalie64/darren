#include "yacfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>

static void ino_to_path(const char *metadir, uint64_t ino, char *out, size_t outsz) {
    snprintf(out, outsz, "%s/%020lu.ino", metadir, ino);
}

static int read_fully(int fd, void *buf, size_t len) {
    size_t total = 0;
    while (total < len) {
        ssize_t n = read(fd, (uint8_t*)buf + total, len - total);
        if (n <= 0) return -1;
        total += n;
    }
    return 0;
}

static int write_fully(int fd, const void *buf, size_t len) {
    size_t total = 0;
    while (total < len) {
        ssize_t n = write(fd, (const uint8_t*)buf + total, len - total);
        if (n <= 0) return -1;
        total += n;
    }
    return 0;
}

int yacfs_meta_save(struct yacfs_state *st, struct yacfs_inode *inode, uint64_t *blocks) {
    char path[4096];
    ino_to_path(st->meta_dir, inode->ino, path, sizeof(path));

    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -errno;

    if (write_fully(fd, inode, sizeof(*inode)) < 0) {
        close(fd); return -EIO;
    }
    size_t blk_bytes = inode->nblocks * sizeof(uint64_t);
    if (blk_bytes > 0 && write_fully(fd, blocks, blk_bytes) < 0) {
        close(fd); return -EIO;
    }
    close(fd);
    return 0;
}

struct yacfs_inode *yacfs_meta_load(struct yacfs_state *st, uint64_t ino, uint64_t **blocks) {
    char path[4096];
    ino_to_path(st->meta_dir, ino, path, sizeof(path));

    int fd = open(path, O_RDONLY);
    if (fd < 0) return NULL;

    struct yacfs_inode *inode = malloc(sizeof(*inode));
    if (!inode) { close(fd); return NULL; }

    if (read_fully(fd, inode, sizeof(*inode)) < 0) {
        free(inode); close(fd); return NULL;
    }

    if (inode->nblocks > 0) {
        *blocks = malloc(inode->nblocks * sizeof(uint64_t));
        if (!*blocks) { free(inode); close(fd); return NULL; }
        if (read_fully(fd, *blocks, inode->nblocks * sizeof(uint64_t)) < 0) {
            free(inode); free(*blocks); *blocks = NULL; close(fd); return NULL;
        }
    } else {
        *blocks = NULL;
    }
    close(fd);
    return inode;
}

int yacfs_meta_delete(struct yacfs_state *st, uint64_t ino) {
    char path[4096];
    ino_to_path(st->meta_dir, ino, path, sizeof(path));
    if (unlink(path) < 0) return -errno;
    return 0;
}

int yacfs_meta_list(struct yacfs_state *st, uint64_t **inos, int *count) {
    DIR *d = opendir(st->meta_dir);
    if (!d) return -errno;

    size_t cap = 256;
    *inos = malloc(cap * sizeof(uint64_t));
    int n = 0;
    struct dirent *de;
    while ((de = readdir(d)) != NULL) {
        if (de->d_type != DT_REG) continue;
        char *dot = strrchr(de->d_name, '.');
        if (!dot || strcmp(dot, ".ino") != 0) continue;
        uint64_t ino = strtoull(de->d_name, NULL, 10);
        if (n >= (int)cap) {
            cap *= 2;
            *inos = realloc(*inos, cap * sizeof(uint64_t));
        }
        (*inos)[n++] = ino;
    }
    closedir(d);
    *count = n;
    return 0;
}
