#include "Transforms.h"

// -------------- picture transformation function wrappers -------------- \\

void invert_picture_wrapper(struct picture *pic, const char *unused){
  printf("calling invert\n");
  invert_picture(pic);
}

void grayscale_picture_wrapper(struct picture *pic, const char *unused){
  printf("calling grayscale\n");
  grayscale_picture(pic);
}

void rotate_picture_wrapper(struct picture *pic, const char *extra_arg){
  int angle = atoi(extra_arg);
  printf("calling rotate (%i)\n", angle);
  rotate_picture(pic, angle);
}

void flip_picture_wrapper(struct picture *pic, const char *extra_arg){
  char plane = extra_arg[0];
  printf("calling flip (%c)\n", plane);
  flip_picture(pic, plane);
}

void blur_picture_wrapper(struct picture *pic, const char *unused){
  printf("calling blur\n");
  blur_picture(pic);
}

// ------------------------------------------------------------------------ \\

