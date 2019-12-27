#ifndef TRUE_CONCURRENCY_CR618_COMMANDS_H
#define TRUE_CONCURRENCY_CR618_COMMANDS_H

#include "PicStore.h"
#include <string.h>

// size of look-up table (for safe IO error reporting)
#define NO_CMDS (sizeof(pcmds) / sizeof(pcmds[0]))

typedef void (*const command_function)(struct pic_store *, const char *, const char *);


void liststore_wrapper(struct pic_store *store, const char *filename, const char *path);
void load_wrapper(struct pic_store *store, const char *filename, const char *path);
void unload_wrapper(struct pic_store *store, const char *filename, const char *path);
void save_wrapper(struct pic_store *store, const char *filename, const char *path);
bool is_jpeg_format(const char *path);
char *get_filename_from_path(char *arg);


// list of all possible commands
static char *pcmd_strings[] = {
        "liststore",
        "load",
        "unload",
        "save"
};

static command_function pcmds[] = {
        liststore_wrapper,
        load_wrapper,
        unload_wrapper,
        save_wrapper
};

#endif //TRUE_CONCURRENCY_CR618_COMMANDS_H
