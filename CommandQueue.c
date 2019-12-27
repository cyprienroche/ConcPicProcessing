#include "CommandQueue.h"

void init_queue_command(struct command_queue *queue) {
  queue->head = malloc(sizeof(struct queue_elem));
  queue->head->next = queue->tail;
  queue->tail = NULL;

  pthread_mutex_init(&queue->head_lock, NULL);
  pthread_mutex_init(&queue->tail_lock, NULL);
}

bool queue_put_command(struct command_queue *queue, int value) {
  struct queue_elem *elem = malloc(sizeof(struct queue_elem));
  if(elem == NULL){
    printf("ERROR: out of memory!\n");
    return false;
  }
  elem->value = value;
  elem->next = NULL;
  pthread_mutex_init(&elem->elem_lock, NULL);
  pthread_mutex_lock(&queue->tail_lock);

  // special case for empty queue
  if(queue->tail == NULL){
    pthread_mutex_lock(&queue->head_lock);
    set_first_elem(queue, elem);
    queue->tail = elem;
    pthread_mutex_unlock(&queue->head_lock);
    pthread_mutex_unlock(&queue->tail_lock);
    return true;
  }

  queue->tail->next = elem;
  queue->tail = elem;
  pthread_mutex_unlock(&queue->tail_lock);
  return true;
}

struct queue_elem *queue_get_command(struct command_queue *queue) {
  struct queue_elem *elem, *next;

  pthread_mutex_lock(&queue->head_lock);
  if (is_empty_queue(queue)) {
    pthread_mutex_unlock(&queue->head_lock);
    return NULL;
  }
  elem = get_first_elem(queue);
  next = elem->next;

  set_first_elem(queue, next);
  pthread_mutex_unlock(&queue->head_lock);

  elem->next = NULL;
  return elem;
}

void queue_free(struct command_queue *queue) {

  pthread_mutex_lock(&queue->head_lock);
  pthread_mutex_lock(&queue->tail_lock);

  struct queue_elem *elem = NULL;
  struct queue_elem *next = queue->head;

  while (next != NULL) {
    elem = next;
    next = elem->next;
    free(elem);
  }

  pthread_mutex_unlock(&queue->tail_lock);
  pthread_mutex_unlock(&queue->head_lock);

  pthread_mutex_destroy(&queue->head_lock);
  pthread_mutex_destroy(&queue->tail_lock);

}

struct queue_elem *get_first_elem(const struct command_queue *queue) {
  // lock acquired already
  return queue->head->next;
}

void set_first_elem(const struct command_queue *queue, struct queue_elem *elem) {
  // lock acquired already
  queue->head->next = elem;
}

bool is_empty_queue(struct command_queue *queue) {
  // lock acquired already
  return get_first_elem(queue) == NULL;
}

