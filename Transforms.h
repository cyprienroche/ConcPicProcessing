#ifndef TRUE_CONCURRENCY_CR618_TRANSFORMS_H
#define TRUE_CONCURRENCY_CR618_TRANSFORMS_H

#include "Picture.h"
#include "PicProcess.h"

// size of look-up table (for safe IO error reporting)
#define NO_TRFS (sizeof(trfs) / sizeof(trfs[0]))

typedef void (*const transform_function)(struct picture *, const char *);

void invert_picture_wrapper(struct picture *pic, const char *unused);
void grayscale_picture_wrapper(struct picture *pic, const char *unused);
void rotate_picture_wrapper(struct picture *pic, const char *extra_arg);
void flip_picture_wrapper(struct picture *pic, const char *extra_arg);
void blur_picture_wrapper(struct picture *pic, const char *unused);

// list of all possible picture transformations
static char *trf_strings[] = {
        "invert",
        "grayscale",
        "rotate",
        "flip",
        "blur"
};

static transform_function trfs[] = {
        invert_picture_wrapper,
        grayscale_picture_wrapper,
        rotate_picture_wrapper,
        flip_picture_wrapper,
        blur_picture_wrapper
};

#endif //TRUE_CONCURRENCY_CR618_TRANSFORMS_H
