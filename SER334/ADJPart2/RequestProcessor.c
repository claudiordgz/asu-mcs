#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "RequestProcessor.h"
#include "queue.h"

enum Door {
    A=0,
    B
};

enum Room {
    Inner=0,
    Outer
};

struct Request {
  enum Door door;
  enum Room room;
  int access_level;
};
typedef struct Request Request;

struct RequestProcessor {
  pthread_mutex_t lock;
  Queue *requests;
  int n_requests;
};

RequestProcessor* create_request_processor() {
  RequestProcessor* rp = malloc(sizeof(RequestProcessor));
  rp->requests = create_queue();
  rp->n_requests = 0;
  pthread_mutex_init(&rp->lock, NULL);
  return rp;
}
void destroy_request_processor(RequestProcessor **rp) {
  pthread_mutex_lock(&(*rp)->lock);
  destroy_queue(&(*rp)->requests, free_request);
  pthread_mutex_unlock(&(*rp)->lock);
  pthread_mutex_destroy(&(*rp)->lock);
  free(*rp);
}
void add_request(RequestProcessor *rp, int room, int access_level, int door, int* data_return) {
  Request *request = malloc(sizeof(Request));
  request->room = room;
  request->access_level = access_level;
  request->door = door;

  pthread_mutex_lock(&rp->lock);
    enqueue(rp->requests, request);
    rp->n_requests++;
  pthread_mutex_unlock(&rp->lock);
}
void free_request(void **data) {
  struct Request *r = *data;
  free(r);
  *data = NULL;
  r = NULL;
}
void print_request(void *data) {
  struct Request *r = data;
  printf("%s %s\n", r->door == A ? "A" : "B", r->room == Inner ? "inner" : "outer");
}
