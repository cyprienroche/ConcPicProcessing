#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "Utils.h"
#include "Picture.h"
#include "List.h"


#define NO_RGB_COMPONENTS 3
#define BLUR_REGION_SIZE 9

#define NO_EXPERIMENT 7

typedef void (*blur_function)(struct picture *, int);

unsigned long measure_time(void (*const function)(struct picture *, int), struct picture *pic, int extra_arg);

void blur_seq(struct picture *pic, int unused);

void blur_thread_per_pix(struct picture *pic, int unsued);

void blur_thread_per_row(struct picture *pic, int unused);

void blur_thread_per_col(struct picture *pic, int unused);

void blur_thread_per_sector(struct picture *pic, int sector_ratio);

struct blur_pixel_args {
  struct picture **original;
  struct picture *copy;
  int i;
  int j;
};

/* blurs one give pixel, specified by i j. */
void blur_pixel(struct picture **original, struct picture *copy, int i, int j) {
  struct pixel rgb;
  int sum_red = 0;
  int sum_green = 0;
  int sum_blue = 0;

  for (int n = -1; n <= 1; n++) {
    for (int m = -1; m <= 1; m++) {
      rgb = get_pixel(copy, i + n, j + m);
      sum_red += rgb.red;
      sum_green += rgb.green;
      sum_blue += rgb.blue;
    }
  }

  rgb.red = sum_red / BLUR_REGION_SIZE;
  rgb.green = sum_green / BLUR_REGION_SIZE;
  rgb.blue = sum_blue / BLUR_REGION_SIZE;

  set_pixel(*original, i, j, &rgb);
}

void init_tmp_and_list(const struct picture *pic, struct list *list, struct picture *tmp) {
  if (list != NULL) {
    init_list(list);
  }
  tmp->img = copy_image(pic->img);
  tmp->width = pic->width;
  tmp->height = pic->height;
}

struct blur_pixel_args *init_arg(struct picture **pic, struct picture *tmp, int i, int j) {
  struct blur_pixel_args *arg = malloc(sizeof(struct blur_pixel_args));
  if(arg == NULL){
    printf("ERROR: out of memory!\n");
    return NULL;
  }
  arg->original = pic;
  arg->copy = tmp;
  arg->i = i;
  arg->j = j;
  return arg;
}

void delegate_thread(pthread_t *thread, struct list *list, void *(*function)(void *), struct blur_pixel_args *arg) {
  insert(list, thread);
  pthread_create(thread, NULL, function, arg);
}

void *blur_per_pix_work(void *args) {
  struct blur_pixel_args *arg = (struct blur_pixel_args *) args;
  blur_pixel(arg->original, arg->copy, arg->i, arg->j);
  free(args);
  return NULL;
}

void *blur_per_row_work(void *args) {
  struct blur_pixel_args *arg = (struct blur_pixel_args *) args;
  for (int j = 1; j < arg->copy->height - 1; j++) {
    blur_pixel(arg->original, arg->copy, arg->i, j);
  }
  free(args);
  return NULL;
}

void *blur_per_col_work(void *args) {
  struct blur_pixel_args *arg = (struct blur_pixel_args *) args;
  for (int i = 1; i < arg->copy->width - 1; i++) {
    blur_pixel(arg->original, arg->copy, i, arg->j);
  }
  free(args);
  return NULL;
}

// ---------- MAIN PROGRAM ---------- \\

/* appends to the files used in the experiment the data gathered over one run. */
void get_data(char *result_path, blur_function function, struct picture *picture, int extra_arg) {
  FILE *f = fopen(result_path, "a");
  unsigned long ticks = measure_time(function, picture, extra_arg);
  double cpu_time_used = ((double) ticks) / CLOCKS_PER_SEC;
  fprintf(f, "width: %d, height: %d, time (s): %f, clock ticks: %lu \n",
          picture->width, picture->height, cpu_time_used, ticks);
  fclose(f);
}

static struct {
  char *path;
  blur_function function;
  int extra_arg;
} results[NO_EXPERIMENT] = {{"blur_experiment_result/sequential.txt", blur_seq, 0},
                {"blur_experiment_result/pixel.txt", blur_thread_per_pix, 0},
                {"blur_experiment_result/row.txt", blur_thread_per_row, 0},
                {"blur_experiment_result/column.txt", blur_thread_per_col, 0},
                {"blur_experiment_result/sectorfull.txt", blur_thread_per_sector, 1},
                {"blur_experiment_result/sectorhalf.txt", blur_thread_per_sector, 2},
                {"blur_experiment_result/sectorquarter.txt", blur_thread_per_sector,4}
};

/* empties the content of every file. */
void clear_all_files() {
  for (int i = 0; i < NO_EXPERIMENT; ++i) {
    FILE *file = fopen(results[i].path, "w");
    fclose(file);
  }
}

int main(int argc, char **argv) {

  printf("Support Code for Running the Blur Optimisation Experiments... \n");
  char *path = "/Users/cyprienroche/Desktop/U.jpg";
  struct picture *picture = malloc(sizeof(struct picture));
  if(picture == NULL){
    printf("ERROR: out of memory!\n");
    return 0;
  }
  init_picture_from_file(picture, path);
  int x = 1;
  while (x > 0) {
    for (int i = 0; i < NO_EXPERIMENT; ++i) {
      get_data(results[i].path, results[i].function, picture, results[i].extra_arg);
    }
    x--;
  }
//  clear_all_files();
  free(picture);
  return 0;
}

/* inspired from: https://www.geeksforgeeks.org/how-to-measure-time-taken-by-a-program-in-c/ */
unsigned long measure_time(blur_function function, struct picture *pic, int extra_arg) {
  clock_t start, end;
  unsigned long clock_ticks;

  start = clock();
  /* Do the work. */
  function(pic, extra_arg);

  end = clock();
  clock_ticks = (end - start);
  return clock_ticks;
}

void blur_seq(struct picture *pic, int unused) {
  struct picture tmp;
  init_tmp_and_list(pic, NULL, &tmp);

  for (int i = 1; i < tmp.width - 1; i++) {
    for (int j = 1; j < tmp.height - 1; j++) {
      blur_pixel(&pic, &tmp, i, j);
    }
  }
  clear_picture(&tmp);
}

void blur_thread_per_pix(struct picture *pic, int unsued) {
  struct list list;
  struct picture tmp;
  init_tmp_and_list(pic, &list, &tmp);

  for (int i = 1; i < tmp.width - 1; i++) {
    for (int j = 1; j < tmp.height - 1; j++) {
      struct blur_pixel_args *arg = init_arg(&pic, &tmp, i, j);
      pthread_t thread;
      delegate_thread(&thread, &list, blur_per_pix_work, arg);
    }
  }

  join_and_free_all(&list);
  clear_picture(&tmp);
}

void blur_thread_per_row(struct picture *pic, int unused) {
  struct list list;
  struct picture tmp;
  init_tmp_and_list(pic, &list, &tmp);

  for (int i = 1; i < tmp.width - 1; i++) {
    struct blur_pixel_args *arg = init_arg(&pic, &tmp, i, 0);
    pthread_t thread;
    delegate_thread(&thread, &list, blur_per_row_work, arg);
  }

  join_and_free_all(&list);
  clear_picture(&tmp);
}


void blur_thread_per_col(struct picture *pic, int unused) {
  struct list list;
  struct picture tmp;
  init_tmp_and_list(pic, &list, &tmp);

  for (int j = 1; j < tmp.height - 1; j++) {
    struct blur_pixel_args *arg = init_arg(&pic, &tmp, 0, j);
    pthread_t thread;
    delegate_thread(&thread, &list, blur_per_col_work, arg);
  }

  join_and_free_all(&list);
  clear_picture(&tmp);

}

void blur_thread_per_sector(struct picture *pic, int sector_ratio) {
  struct list list;
  struct picture tmp;
  init_tmp_and_list(pic, &list, &tmp);

  int unit = tmp.width / sector_ratio;
  for (int i = 1; i < tmp.width - 1; i += unit) {
    for (int j = 1; j < tmp.height - 1; j += unit) {
      struct blur_pixel_args *arg = init_arg(&pic, &tmp, i, j);
      pthread_t thread;
      delegate_thread(&thread, &list, blur_per_pix_work, arg);
    }
  }

  join_and_free_all(&list);
  clear_picture(&tmp);
}


