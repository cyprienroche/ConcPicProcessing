#include <assert.h>
#include <string.h>
#include "PicStore.h"

void init_picstore(struct pic_store *pstore){
  assert(pstore != NULL);
  index_init(&pstore->index, NULL, 0);
}    

void print_picstore(struct pic_store *pstore){
  assert(pstore != NULL);
  index_print(&pstore->index, '<');
}

bool load_picture(struct pic_store *pstore, char *filename, const char *path) {
  assert(pstore != NULL);
  if (filename == NULL) {
    return false;
  }
  struct picture *picture = malloc(sizeof(struct picture));
  if(picture == NULL){
    printf("ERROR: out of memory!\n");
    return false;
  }
  //crete thread and give him work
  if (!init_picture_from_file(picture, path)) {
    return false;
  }
  return index_insert(&pstore->index, djb_hash(filename), filename, picture);
}

bool unload_picture(struct pic_store *pstore, const char *filename){
  assert(pstore != NULL);
  if (filename == NULL) {
    return false;
  }
  return index_remove(&pstore->index, djb_hash(filename));
}

bool save_picture(struct pic_store *pstore, const char *filename, const char *path){
  assert(pstore != NULL);
  if (filename == NULL) {
    return false;
  }
  struct picture *picture = index_search(&pstore->index, djb_hash(filename));
  if (picture == NULL) {
    return false;
  }
  return save_picture_to_file(picture, path);
}

void free_picstore(struct pic_store *pstore){
  assert(pstore != NULL);
  index_free(&pstore->index);
}

void transform_picture(struct pic_store *pstore, const char *filename, transform_function function, const char *args) {
  if (filename == NULL) {
    return;
  }
  struct picture *picture = index_search(&pstore->index, djb_hash(filename));
  if (picture == NULL) {
    printf("Error. Picture not found in store\n");
    return;
  }
  function(picture, args);
}

char *mcopy_string(const char *str) {
  int n = (int) strlen(str) + 1;
  char *cp = malloc(n);
  if(cp == NULL){
    printf("ERROR: out of memory!\n");
    return NULL;
  }
  strncpy(cp, str, n);
  return cp;
}