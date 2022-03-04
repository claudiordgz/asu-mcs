#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct node {
  void *data;
  struct node *next;
  int priority;
};

Queue* create_queue() {
  Queue* q = malloc(sizeof(Queue));
  q->front = NULL;
  q->back = NULL;
  q->size = 0;
  return q;
}
void enqueue(Queue *q, void *data, int priority) {
  Node *n = malloc(sizeof(Node));
  n->data = data;
  n->next = NULL;
  if (q->size == 0) {
    q->front = n;
    q->back = n;
  } else {
    if (priority < q->front->priority) {
      q->front = q->front->next;
      n->next = q->front;
      q->front = n;
    } else if (priority == q->front->priority) {
      Node *curr = q->front;
      while (curr->next != NULL && curr->next->priority == priority) {
        curr = curr->next;
      }
      n->next = curr->next;
      curr->next = n;
    } else {
      Node *curr = q->front;
      while (curr->next != NULL && curr->next->priority < priority) {
        curr = curr->next;
      }
      n->next = curr->next;
      curr->next = n;
    }
  }
  q->size++;
}
void* pop(Queue *q) {
  if (q->size == 0) {
    return NULL;
  }
  Node *n = q->front;
  q->front = n->next;
  q->size--;
  void *data = n->data;
  free(n);
  return data;
}
void* peek(Queue *q) {
  if (q->size == 0) {
    return NULL;
  }
  return q->front->data;
}
int size(Queue *q) {
  return q->size;
}
void print_queue(Queue *q, void (*print_data)(void *)) {
  Node *n = q->front;
  while (n != NULL) {
    print_data(n->data);
    n = n->next;
  }
}
void destroy_queue(Queue **q, void (*free_data)(void **)) {
  Node *n = (*q)->front;
  while (n != NULL) {
    Node *next = n->next;
    if (free_data != NULL) {
      free_data(&n->data);
    }
    free(n);
    n = next;
  }
  free(*q);
  *q = NULL;
}
