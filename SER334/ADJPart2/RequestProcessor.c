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
  int *response;
};
typedef struct Request Request;

void process_request(Request *request);

struct QueueWorker {
  pthread_mutex_t lock;
  pthread_t thread;
  Queue *requests;
  int run_worker;
};
typedef struct QueueWorker QueueWorker;
QueueWorker* queue_worker = NULL;
QueueWorker* create_queue_worker();
void destroy_queue_worker(QueueWorker **worker);
void* process_queue(void *result);

////////////////////////////////////////////////////////////////////////////////
// QueueWorker functions
////////////////////////////////////////////////////////////////////////////////
QueueWorker* create_queue_worker() {
  QueueWorker *worker = malloc(sizeof(QueueWorker));
  worker->requests = create_queue();
  pthread_mutex_init(&worker->lock, NULL);
  worker->run_worker = 1;
  return worker;
}
void destroy_queue_worker(QueueWorker **worker) {
  destroy_queue(&(*worker)->requests, free_request);
  pthread_mutex_destroy(&(*worker)->lock);
  free(*worker);
  *worker = NULL;
}
void* process_queue(void *result) {
  QueueWorker *worker = (QueueWorker *)result;
  while (worker->run_worker) {
    Request *request = NULL;
    pthread_mutex_lock(&worker->lock);
      if (size(worker->requests) > 0) {
        request = (Request *)pop(worker->requests);
      }
    pthread_mutex_unlock(&worker->lock);
    if (request != NULL) {
      print_request(request);
      process_request(request);
      free_request((void*)request);
    }
  }
  pthread_mutex_lock(&worker->lock);
    printf("Queue worker exiting\n");
    while (size(worker->requests) > 0) {
      Request *request = (Request *)pop(worker->requests);
      *(request->response) = -1;
    }
  pthread_mutex_unlock(&worker->lock);
  return NULL;
}

struct RequestProcessor {
  int n_requests;
  QueueWorker *worker;
};
typedef struct RequestProcessor RequestProcessor;
////////////////////////////////////////////////////////////////////////////////
// RequestProcessor functions
////////////////////////////////////////////////////////////////////////////////
RequestProcessor* create_request_processor() {
  RequestProcessor *request_processor = malloc(sizeof(RequestProcessor));
  request_processor->n_requests = 0;
  request_processor->worker = create_queue_worker();
  pthread_create(&request_processor->worker->thread, NULL, process_queue, request_processor->worker);
  return request_processor;
}
void destroy_request_processor(RequestProcessor **rp) {
  printf("Destroying request processor\n");
  (*rp)->worker->run_worker = 0;
  pthread_join((*rp)->worker->thread, NULL);
  destroy_queue_worker(&(*rp)->worker);
  free(*rp);
  *rp = NULL;
}
void add_request(RequestProcessor *rp, int room, int access_level, int door, int* data_return) {
  printf("Adding request room %d, door %d, %d access\n", room, door, access_level);
  Request *request = malloc(sizeof(Request));
  request->room = room;
  request->access_level = access_level;
  request->door = door;
  request->response = data_return;

  // Add request to queue 
  // only if worker is running
  if (rp->worker->run_worker) {
    pthread_mutex_lock(&rp->worker->lock);
      enqueue(rp->worker->requests, request, request->access_level);
      rp->n_requests++;
    pthread_mutex_unlock(&rp->worker->lock);
  }
}
void free_request(void **data) {
  printf("Freeing request\n");
  struct Request *r = *data;
  free(r);
  *data = NULL;
  r = NULL;
}
void print_request(void *data) {
  struct Request *r = data;
  printf("Door: %s, From: %s\n", r->door == A ? "A" : "B", r->room == Inner ? "inner" : "outer");
}
////////////////////////////////////////////////////////////////////////////////
// Request functions
////////////////////////////////////////////////////////////////////////////////
void process_request(Request *request) {
  if (request->room == Inner) {
    // 2. get answer from control room 
    *request->response = 1;
  } else {
    *request->response = 0;
  }
  if (request->access_level == 1) {
    if (request->door == A) {
    } else {
      if (request->room == Inner) {
        *request->response = 0;
      } else {
        *request->response = 1;
      }
    }
  } else {
    *request->response = 1;
  }
}