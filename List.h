#ifndef TRUE_CONCURRENCY_CR618_LIST_H
#define TRUE_CONCURRENCY_CR618_LIST_H

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

struct list {
  struct list_elem *head;
  struct list_elem *tail;

  pthread_mutex_t head_lock;
  pthread_mutex_t tail_lock;
};

struct list_elem {
  pthread_t *thread;

  struct list_elem *next;
};

void init_list(struct list *list);
bool insert(struct list *list, pthread_t *thread);
bool join_and_free_all(struct list *list);
void free_all(struct list *list);


#endif //TRUE_CONCURRENCY_CR618_LIST_H
