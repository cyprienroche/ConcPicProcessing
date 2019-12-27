#ifndef TRUE_CONCURRENCY_CR618_COMMANDQUEUE_H
#define TRUE_CONCURRENCY_CR618_COMMANDQUEUE_H
#include <stddef.h>
#include <stdio.h>
#include <mm_malloc.h>
#include <stdbool.h>
#include <pthread.h>

struct command_queue {
  struct queue_elem *head;
  struct queue_elem *tail;

  pthread_mutex_t head_lock;
  pthread_mutex_t tail_lock;
};

struct queue_elem {
  int value;
  struct queue_elem *next;

  pthread_mutex_t elem_lock;
};


void init_queue_command(struct command_queue *queue);

bool queue_put_command(struct command_queue *queue, int value);

struct queue_elem *queue_get_command(struct command_queue *queue);

void queue_free(struct command_queue *queue);

bool is_empty_queue(struct command_queue *queue);

struct queue_elem *get_first_elem(const struct command_queue *queue);

void set_first_elem(const struct command_queue *queue, struct queue_elem *elem);

#endif //TRUE_CONCURRENCY_CR618_COMMANDQUEUE_H

