#include "yacfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

static void snap_path(struct yacfs_state *st, const char *name, char *out, size_t outsz) {
    if (name)
        snprintf(out, outsz, "%s/%s", st->snap_dir, name);
    else
        snprintf(out, outsz, "%s", st->snap_dir);
}

static int copy_file(const char *src, const char *dst) {
    int sfd = open(src, O_RDONLY);
    if (sfd < 0) return -errno;
    int dfd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dfd < 0) { close(sfd); return -errno; }
    char buf[262144];
    ssize_t n;
    while ((n = read(sfd, buf, sizeof(buf))) > 0) {
        ssize_t w = 0;
        while (w < n) {
            ssize_t r = write(dfd, buf + w, n - w);
            if (r <= 0) break;
            w += r;
        }
    }
    close(sfd);
    close(dfd);
    return 0;
}

int yacfs_snap_create(struct yacfs_state *st, const char *name) {
    char spath[4096];
    snap_path(st, name, spath, sizeof(spath));

    struct stat sb;
    if (stat(spath, &sb) == 0) return -EEXIST;
    if (mkdir(spath, 0755) < 0) return -errno;

    DIR *d = opendir(st->meta_dir);
    if (!d) { rmdir(spath); return -errno; }

    struct dirent *de;
    while ((de = readdir(d)) != NULL) {
        if (de->d_type != DT_REG) continue;
        char src[4096], dst[4096];
        snprintf(src, sizeof(src), "%s/%s", st->meta_dir, de->d_name);
        snprintf(dst, sizeof(dst), "%s/%s", spath, de->d_name);
        copy_file(src, dst);
    }
    closedir(d);
    return 0;
}

int yacfs_snap_list(struct yacfs_state *st, char ***names, int *count) {
    DIR *d = opendir(st->snap_dir);
    if (!d) return -errno;

    size_t cap = 16;
    *names = malloc(cap * sizeof(char*));
    int n = 0;

    struct dirent *de;
    while ((de = readdir(d)) != NULL) {
        if (de->d_name[0] == '.') continue;
        if (de->d_type != DT_DIR) continue;
        if (n >= (int)cap) {
            cap *= 2;
            *names = realloc(*names, cap * sizeof(char*));
        }
        (*names)[n++] = strdup(de->d_name);
    }
    closedir(d);
    *count = n;
    return 0;
}

int yacfs_snap_rollback(struct yacfs_state *st, const char *name) {
    char spath[4096];
    snap_path(st, name, spath, sizeof(spath));

    struct stat sb;
    if (stat(spath, &sb) < 0) return -ENOENT;

    char tmpdir[4096];
    snprintf(tmpdir, sizeof(tmpdir), "%s.tmp", st->meta_dir);
    if (rename(st->meta_dir, tmpdir) < 0) return -errno;

    if (rename(spath, st->meta_dir) < 0) {
        rename(tmpdir, st->meta_dir);
        return -errno;
    }

    char new_snap[4096];
    snprintf(new_snap, sizeof(new_snap), "%s/rollback_%s", st->snap_dir, name);
    rename(tmpdir, new_snap);
    return 0;
}

void yacfs_snap_free_list(char ***names, int count) {
    for (int i = 0; i < count; i++)
        free((*names)[i]);
    free(*names);
    *names = NULL;
}
