#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Utils.h"
#include "PicStore.h"
#include "Commands.h"
#include "Transforms.h"

#define EOL "\n"
#define DELIMITER " "

static bool transform(struct pic_store *pstore, const char *filename,
                      const char *process, const char *args);

static bool pstore_command(struct pic_store *pstore, const char *filename,
                           const char *process, const char *args);

static bool filename_is_second_arg(const char *command);
static int get_command_no(const char *process, int number_of_commands, char **command_names);

void get_arguments(char *command, char **filename, char **extra_arg);

void load_arg(char **filename, const char *path);

// ---------- MAIN PROGRAM ---------- \\

int main(int argc, char **argv) {

  printf("Running the Interactive C Picture Processing Library... \n");

  struct pic_store store;
  init_picstore(&store);
  char *filename;
  char *extra_arg;
  char command[350];

  while (argc > 1) {
    argc--;
    char *path = argv[argc];
    load_arg(&filename, path);
    pstore_command(&store, filename, "load" ,path);
  }

  while (true) {

    get_arguments(command, &filename, &extra_arg);

    // exit
    if (strcmp(command, "exit") == 0) {
      printf("\nExiting the Interactive C Picture Processing Library... \n");
      break;
    }

    // liststore
    // load <file_path > <file_name >
    // unload <file_name >
    // save <file_name > <file_path >
    pstore_command(&store, filename, command, extra_arg)
    //invert <file_name>
    //grayscale <file_name>
    //rotate [90|180|270] <file_name>
    //flip [H|V] <file_name>
    //blur <file_name>
    || transform(&store, filename, command, extra_arg);
  }

  free_picstore(&store);
  return 0;
}

void load_arg(char **filename, const char *path) {
  char *path_cp = mcopy_string(path);
  *filename = get_filename_from_path(path_cp);
  free(path_cp);
}

void get_arguments(char *command, char **filename, char **extra_arg) {
  fgets(command, 350, stdin);
  strtok(command, EOL);
  strtok(command, DELIMITER);

  if (filename_is_second_arg(command)) {
    *extra_arg = strtok(NULL, DELIMITER);
    *filename = strtok(NULL, DELIMITER);
  } else {
    *filename = strtok(NULL, DELIMITER);
    *extra_arg = strtok(NULL, DELIMITER);
  }
}

static bool filename_is_second_arg(const char *command) {
  return strcmp(command, "load") == 0
         || strcmp(command, "rotate") == 0
         || strcmp(command, "flip") == 0;
}

// char *path = "/Users/cyprienroche/Desktop/colours.jpg";

static bool transform(struct pic_store *pstore, const char *filename, const char *process, const char *args) {
  // identify the picture transformation to run
  int trf_no = get_command_no(process, NO_TRFS, trf_strings);

  // IO error check
  if (trf_no == NO_TRFS) {
    return false;
  }

  // dispatch to appropriate picture transformation function
  transform_picture(pstore, filename, trfs[trf_no], args);
  return true;
}

static bool pstore_command(struct pic_store *pstore, const char *filename, const char *process, const char *args) {
  // identify the command to run
  int cmd_no = get_command_no(process, NO_CMDS, pcmd_strings);

  // IO error check
  if (cmd_no == NO_CMDS) {
    return false;
  }

  // dispatch to appropriate command function
  pcmds[cmd_no](pstore, filename, args);
  return true;
}

static int get_command_no(const char *process, int number_of_commands, char **command_names) {
  // identify the command to run
  int num = 0;
  while (num < number_of_commands && strcmp(process, command_names[num])) {
    num++;
  }
  
  return num;
}
