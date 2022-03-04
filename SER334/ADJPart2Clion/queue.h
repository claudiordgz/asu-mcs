#ifndef QUEUE_H
#define QUEUE_H

typedef struct node Node;
struct Queue {
  int size;
  Node *front;
  Node *back;
};
typedef struct Queue Queue;

Queue * create_queue();
void enqueue(Queue *q, void *data, int priority);
void* pop(Queue *q);
void* peek(Queue *q);
int size(Queue *q);
void print_queue(Queue *q, void (*print_data)(void *));
void destroy_queue(Queue **q, void (*free_data)(void **));

#endif