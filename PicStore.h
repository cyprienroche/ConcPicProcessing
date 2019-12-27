#ifndef PICSTORE_H
#define PICSTORE_H

#include "Picture.h"
#include "Utils.h"
#include "Index.h"
#include "Transforms.h"

struct pic_store {
  struct index index;
};

// picture library initialisation 
void init_picstore(struct pic_store *pstore);

// command-line interpreter routines
void print_picstore(struct pic_store *pstore);
bool load_picture(struct pic_store *pstore, char *filename, const char *path);
bool unload_picture(struct pic_store *pstore, const char *filename);
bool save_picture(struct pic_store *pstore, const char *filename, const char *path);
void free_picstore(struct pic_store *pstore);
void transform_picture(struct pic_store *pstore, const char *filename, transform_function function, const char *args);
char *mcopy_string(const char *str);

#endif

