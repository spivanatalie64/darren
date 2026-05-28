#define FUSE_USE_VERSION 31
#include "yacfs.h"
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

static void usage(const char *prog) {
    fprintf(stderr,
        "Usage: %s <mountpoint> -o pool=<dir>[,compress=<level>][,snapdir=<dir>]\n"
        "  pool=<dir>       Backing storage directory (can be specified multiple times with : separator)\n"
        "  compress=<level>  zstd compression level (0=off, default=3, max=19)\n"
        "  snapdir=<dir>     Snapshots directory (default: <pool>/.snapshots)\n"
        "\nExample:\n"
        "  %s /mnt/zfs -o pool=/data/storage,compress=3\n"
        "  %s /mnt/zfs -o pool=/data/disk1:/data/disk2,compress=1\n",
        prog, prog, prog);
}

int main(int argc, char *argv[]) {
    struct yacfs_state state = {0};
    state.compress_level = 1;

    char *pool_str = NULL;
    char *snap_str = NULL;
    char *compress_str = NULL;

    char **new_argv = malloc(argc * sizeof(char*));
    int new_argc = 0;
    new_argv[new_argc++] = argv[0];

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            char *opts = strdup(argv[i + 1]);
            char filtered[4096] = {0};
            char *save, *opt = strtok_r(opts, ",", &save);
            int first = 1;
            while (opt) {
                int is_custom = 0;
                if (strncmp(opt, "pool=", 5) == 0) { pool_str = strdup(opt + 5); is_custom = 1; }
                else if (strncmp(opt, "snapdir=", 8) == 0) { snap_str = strdup(opt + 8); is_custom = 1; }
                else if (strncmp(opt, "compress=", 9) == 0) { compress_str = strdup(opt + 9); is_custom = 1; }

                if (!is_custom) {
                    if (!first) strcat(filtered, ",");
                    strcat(filtered, opt);
                    first = 0;
                }
                opt = strtok_r(NULL, ",", &save);
            }
            free(opts);
            if (filtered[0]) {
                new_argv[new_argc++] = strdup("-o");
                new_argv[new_argc++] = strdup(filtered);
            }
            i++;
        } else {
            new_argv[new_argc++] = argv[i];
        }
    }
    new_argv[new_argc] = NULL;

    if (!pool_str) {
        usage(argv[0]);
        return 1;
    }

    if (compress_str) {
        state.compress_level = atoi(compress_str);
        free(compress_str);
    }

    {
        char *save, *p = strtok_r(pool_str, ":", &save);
        state.npools = 0;
        state.pools = malloc(sizeof(struct pool_dir));
        while (p) {
            state.npools++;
            state.pools = realloc(state.pools, state.npools * sizeof(struct pool_dir));
            strncpy(state.pools[state.npools - 1].path, p, sizeof(state.pools[0].path) - 1);
            state.pools[state.npools - 1].path_len = strlen(p);

            char blk_dir[4096];
            snprintf(blk_dir, sizeof(blk_dir), "%s/blocks", p);
            mkdir(blk_dir, 0755);

            char meta_dir[4096];
            snprintf(meta_dir, sizeof(meta_dir), "%s/meta", p);
            mkdir(meta_dir, 0755);

            char snap_dir[4096];
            if (snap_str)
                snprintf(snap_dir, sizeof(snap_dir), "%s", snap_str);
            else
                snprintf(snap_dir, sizeof(snap_dir), "%s/.snapshots", p);
            mkdir(snap_dir, 0755);

            p = strtok_r(NULL, ":", &save);
        }
        free(pool_str);
    }

    if (state.npools > 0) {
        snprintf(state.blk_dir, sizeof(state.blk_dir), "%s/blocks", state.pools[0].path);
        snprintf(state.meta_dir, sizeof(state.meta_dir), "%s/meta", state.pools[0].path);
        if (snap_str)
            snprintf(state.snap_dir, sizeof(state.snap_dir), "%s", snap_str);
        else
            snprintf(state.snap_dir, sizeof(state.snap_dir), "%s/.snapshots", state.pools[0].path);
    }
    free(snap_str);

    state.next_ino = 2;

    {
        uint64_t *blks;
        struct yacfs_inode *root = yacfs_meta_load(&state, 1, &blks);
        if (!root) {
            struct yacfs_inode root_in = {
                .ino = 1,
                .size = 0,
                .mtime = time(NULL),
                .ctime = time(NULL),
                .mode = S_IFDIR | 0755,
                .uid = 0,
                .gid = 0,
                .nblocks = 0,
                .block_size = BLOCK_SIZE,
                .checksum_type = CHECKSUM_XXH64,
                .compress_type = COMPRESS_ZSTD,
            };
            yacfs_meta_save(&state, &root_in, NULL);
        } else {
            free(blks);
            free(root);
        }
    }

    pthread_mutex_init(&state.lock, NULL);

    int ret = fuse_main(new_argc, new_argv, &yacfs_ops, &state);
    pthread_mutex_destroy(&state.lock);
    free(state.pools);
    for (int i = 0; i < new_argc; i++) {
        if (new_argv[i] != argv[i]) free(new_argv[i]);
    }
    free(new_argv);
    return ret;
}
