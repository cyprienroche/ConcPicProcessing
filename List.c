#include "List.h"
#include <stdio.h>

void init_list(struct list *list) {
  list->head = malloc(sizeof(struct list_elem *));
  list->tail =NULL;

  pthread_mutex_init(&list->head_lock, NULL);
  pthread_mutex_init(&list->tail_lock, NULL);
}

void print(struct list *list) {
  struct list_elem *elem = NULL;
  struct list_elem *next = list->head->next;

  while (next != NULL) {
    elem = next;
    next = elem->next;
    printf("A\n");
  }
}

bool insert(struct list *list, pthread_t *thread) {
  struct list_elem *elem = malloc(sizeof(struct list_elem *));
  if(elem == NULL){
    printf("ERROR: out of memory!\n");
    return false;
  }
  elem->thread = thread;
  elem->next = NULL;

  pthread_mutex_lock(&list->tail_lock);
  if (list->tail == NULL) {
    pthread_mutex_lock(&list->head_lock);
    list->head->next = elem;
    list->tail = elem;
    pthread_mutex_unlock(&list->head_lock);
    pthread_mutex_unlock(&list->tail_lock);
    return true;
  }
  list->tail->next = elem;
  list->tail = elem;
  pthread_mutex_unlock(&list->tail_lock);
  return true;
}

bool join_and_free_all(struct list *list) {
  pthread_mutex_lock(&list->head_lock);
  pthread_mutex_lock(&list->tail_lock);

  struct list_elem *elem = NULL;
  struct list_elem *next = list->head->next;

  while (next != NULL) {
    elem = next;
    next = elem->next;
    pthread_join(*elem->thread, NULL);
    free(elem);
  }

  pthread_mutex_unlock(&list->tail_lock);
  pthread_mutex_unlock(&list->head_lock);

  return true;
}

void free_all(struct list *list) {
  pthread_mutex_lock(&list->head_lock);
  pthread_mutex_lock(&list->tail_lock);

  struct list_elem *elem = NULL;
  struct list_elem *next = list->head;

  while (next != NULL) {
    elem = next;
    next = elem->next;
    free(elem);
  }

  pthread_mutex_unlock(&list->tail_lock);
  pthread_mutex_unlock(&list->head_lock);
}
