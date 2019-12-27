#include "Index.h"

#include <stdio.h>
#include <mm_malloc.h>

//---------------------------------------------------------------------------\\

// Index implementation

void index_init(struct index *idx, int seeds[], int length){
  
  idx->head = NULL;
  idx->tail = NULL;

  pthread_mutex_init(&idx->head_lock, NULL);
  pthread_mutex_init(&idx->head_lock, NULL);

  // insert the seed elements into the index
  for(int i=0; i<length; i++){
    index_insert(idx, seeds[i], NULL, NULL);
  }
  
}

struct picture *index_search(struct index *idx, size_t key){

  // set-up for index traversal
  pthread_mutex_lock(&idx->head_lock);
  struct node *cur = NULL;
  struct node *nxt = idx->head;

  size_t item = 0;

  // traverse the index elements
  while(nxt != NULL && item < key ){

    pthread_mutex_lock(&nxt->node_lock);
    if (nxt == idx->head) {
      pthread_mutex_unlock(&idx->head_lock);
    } else {
      pthread_mutex_unlock(&cur->node_lock);
    }
    cur = nxt;
    nxt = cur->next;
    item = cur->item;

    if(item == key){
      // found it!
      pthread_mutex_unlock(&cur->node_lock);
      return cur->picture;
    }
  }
  
  // item not in the index so, did not find it

  if (cur == NULL) {
    pthread_mutex_unlock(&idx->head_lock);
  } else {
    pthread_mutex_unlock(&cur->node_lock);
  }
  return NULL;
}

bool index_insert(struct index *idx, size_t key, char *filename, struct picture *picture) {
  
  // create basic Node
  struct node *new_node = malloc(sizeof(struct node));
  if(new_node == NULL){
    printf("ERROR: out of memory!\n");
    return false;
  }
  new_node->prev = NULL;
  new_node->next = NULL;
  new_node->item = key;
  new_node->filename = filename;
  new_node->picture = picture;
  pthread_mutex_init(&new_node->node_lock, NULL);
  pthread_mutex_lock(&idx->head_lock);


  // special case for empty index
  if(idx->head == NULL && idx->tail == NULL){
    idx->head = new_node;
    idx->tail = new_node;
    pthread_mutex_unlock(&idx->head_lock);
    return true;
  }

  // set-up for index traversal
  struct node *prv = NULL;
  struct node *cur = NULL;
  struct node *nxt = idx->head;
  
  // traverse the index elements
  while(nxt != NULL){

    pthread_mutex_lock(&nxt->node_lock);
    if (cur != NULL) {
      if (cur == idx->head) {
        pthread_mutex_unlock(&idx->head_lock);
      } else {
        pthread_mutex_unlock(&prv->node_lock);
      }
    }

    prv = cur;
    cur = nxt;
    nxt = cur->next;
    size_t item = cur->item;

    if(item == key){
      // we do not duplicate items in the index
      free(new_node->picture);
      free(new_node);

      pthread_mutex_unlock(&cur->node_lock);
      if (prv != NULL) {
        pthread_mutex_unlock(&prv->node_lock);
      } else {
        pthread_mutex_unlock(&idx->head_lock);
      }
      return false;
    }

    if(item > key){
      // found where the new item should go

      // update new Node pointers (dangles off of list for now)
      new_node->prev = prv;
      new_node->next = cur;

      // update back pointer of current element
      cur->prev = new_node;

      // update forward pointer of prev element (or list head)
      if(prv != NULL){
        prv->next = new_node;
      } else {
        idx->head = new_node;
      }

      pthread_mutex_unlock(&cur->node_lock);
      if (prv != NULL) {
        pthread_mutex_unlock(&prv->node_lock);
      } else {
        pthread_mutex_unlock(&idx->head_lock);
      }

      return true;

    } 
    // goto next element
  }

  // if not inserted in loop then item belongs on the end of the list
  idx->tail->next = new_node;
  new_node->prev = idx->tail;
  idx->tail = new_node;

  if (cur != NULL) {
    pthread_mutex_unlock(&cur->node_lock);
  }
  if (prv != NULL) {
    pthread_mutex_unlock(&prv->node_lock);
  } else {
    pthread_mutex_unlock(&idx->head_lock);
  }
  return true;
  
}


bool index_remove(struct index *idx, size_t key){

  pthread_mutex_lock(&idx->head_lock);


  // set-up for index traversal
  struct node *prv = NULL;
  struct node *cur = NULL;
  struct node *nxt = idx->head;

  // traverse the index elements
  while(nxt != NULL){

    pthread_mutex_lock(&nxt->node_lock);
    if (cur != NULL) {
      if (cur == idx->head) {
        pthread_mutex_unlock(&idx->head_lock);
      } else {
        pthread_mutex_unlock(&prv->node_lock);
      }
    }

    prv = cur;
    cur = nxt;
    nxt = cur->next;
    size_t item = cur->item;

    // check each item
    if(item == key){
      // found the item to be removed

      // update back pointer of next element (or list tail)
      if(nxt != NULL){
        nxt->prev = prv;
      } else {
        idx->tail = prv;
      }
      // update forward pointer of prev element (or list head)
      if(prv != NULL){
        prv->next = nxt;
      } else {
        idx->head = nxt;
      }

      pthread_mutex_unlock(&cur->node_lock);
      if (prv != NULL) {
        pthread_mutex_unlock(&prv->node_lock);
      } else {
        pthread_mutex_unlock(&idx->head_lock);
      }

      // free up memory for Node object
      free(cur->picture);
      free(cur);
      return true;
    }
  }
  // item not in the index, so did not remove it
  if (cur != NULL) {
    pthread_mutex_unlock(&cur->node_lock);
  }
  if (prv != NULL) {
    pthread_mutex_unlock(&prv->node_lock);
  } else {
    pthread_mutex_unlock(&idx->head_lock);
  }
  return false;
  
}

void index_print(struct index *idx, char order){

  struct node *start;

  // configure traversal order
  if(order == '<'){
    start = idx->head;
  } else if(order == '>'){
    start = idx->tail;
  } else {
    printf("ERROR: unrecognised input order %c\n", order);
    return;
  } 
  
  // set-up for index traversal  
  struct node *cur = NULL; 
  struct node *nxt = start;

  // traverse the index elements
  while(nxt != NULL){

    // update next item in the list (depending on traversal order)
    cur = nxt;
    if(order == '<'){
      nxt = cur->next;
    } else {
      nxt = cur->prev;
    }
    
    // print the current item
    printf("%s\n", cur->filename);
  }
  // finish the index traversal
}

/* source = https://codereview.stackexchange.com/questions/85556/simple-string-hashing-algorithm-implementation */
size_t djb_hash(const char* cp) {
  size_t hash = 5381;
  while (*cp)
    hash = 33 * hash ^ (unsigned char) *cp++;
  return hash;
}

void index_free(struct index *idx){

  struct node *start;
  start = idx->head;

  // set-up for index traversal
  struct node *cur = NULL;
  struct node *nxt = start;

  // traverse the index elements
  while(nxt != NULL){

    // update next item in the list
    cur = nxt;
    nxt = cur->next;

    // free the current item
    free(cur->filename);
    free(cur->picture);
    free(cur);
  }
  // finish the index traversal
  pthread_mutex_destroy(&idx->head_lock);
}

