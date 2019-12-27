#ifndef INDEX_H
#define INDEX_H

  #include <stdbool.h>
  #include <pthread.h>

  // The node struct implements a Doubly-Linked-List element
  struct node {

    // index key of the node  
    size_t item;
    char *filename;
    struct picture *picture;

    // pointers to previous/next nodes in the list
    struct node *prev;
    struct node *next;

    pthread_mutex_t node_lock;
  };

  // The index struct provides a wrapper for a Concurrent Doubely-Linked-List 
  // implementation
  
  struct index {

    // pointers to the head and tail of the doubly linked list
    struct node *head;
    struct node *tail;

    pthread_mutex_t head_lock;
  };
  
  // initialise index IDX from the provided array SEEDS of size LENGTH
  void index_init(struct index *idx, int seeds[], int length);

  // look for element with KEY in the index IDX
  struct picture *index_search(struct index *idx, size_t key);

  // add element with KEY (in order) to the index IDX
  bool index_insert(struct index *idx, size_t key, char *filename, struct picture *picture);

  // remove element with KEY from the index IDX
  bool index_remove(struct index *idx, size_t key);

  // output the contents of the index IDX (pretty-printed) in specified ORDER
  void index_print(struct index *idx, char order);

  //D. J. Bernstein hash function
  size_t djb_hash(const char* cp);

  // free the content of the index (struct picture * associated with each node)
  void index_free(struct index *idx);

#endif
