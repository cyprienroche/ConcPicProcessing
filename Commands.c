#include "Commands.h"

void liststore_wrapper(struct pic_store *store, const char *filename, const char *path) {
  print_picstore(store);
}

void load_wrapper(struct pic_store *store, const char *filename, const char *path) {
  char *name = mcopy_string(filename);
  if (!is_jpeg_format(path) || !load_picture(store, name, path)) {
    printf("Error. Could not load picture.\n");
    return;
  }
}

void unload_wrapper(struct pic_store *store, const char *filename, const char *path) {
  if (!unload_picture(store, filename)) {
    printf("Error. Could not unload picture.\n");
  }
}

void save_wrapper(struct pic_store *store, const char *filename, const char *path) {
  if (!save_picture(store, filename, path)) {
    printf("Error. Could not save picture.\n");
    return;
  }
}

bool is_jpeg_format(const char *path) {
  char *temp = mcopy_string(path);
  strtok(temp, ".");
  strtok(NULL, ".");
  bool is_jpg = strcmp(temp, "jpg");
  free(temp);
  return is_jpg;
}

char *get_filename_from_path(char *arg) {
  char *prv;
  char *token = strtok(arg, "/");
  while (token != NULL) {
    prv = token;
    token = strtok(NULL, "/");
  }
  strtok(prv, ".");
  return prv;
}
