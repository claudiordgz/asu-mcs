#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "RequestProcessor.h"
#include "queue.h"

#define DEBUG 0

enum Door {
    A = 0,
    B
};

enum Room {
    Inner = 0,
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
QueueWorker *queue_worker = NULL;

QueueWorker *create_queue_worker();

void destroy_queue_worker(QueueWorker **worker);

void *process_queue(void *result);

////////////////////////////////////////////////////////////////////////////////
// QueueWorker functions
////////////////////////////////////////////////////////////////////////////////
QueueWorker *create_queue_worker() {
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

void *process_queue(void *result) {
    QueueWorker *worker = (QueueWorker *) result;
    while (worker->run_worker) {
        Request *request = NULL;
        pthread_mutex_lock(&worker->lock);
        if (size(worker->requests) > 0) {
            request = (Request *) pop(worker->requests);
        }
        pthread_mutex_unlock(&worker->lock);
        if (request != NULL) {
            if (DEBUG)
                print_request(request);
            process_request(request);
            free(request);
            request = NULL;
        }
    }
    // cancel, approve, deny
    pthread_mutex_lock(&worker->lock);
    if (DEBUG)
        printf("Queue worker exiting\n");
    while (size(worker->requests) > 0) {
        Request *request = (Request *) pop(worker->requests);
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
RequestProcessor *create_request_processor() {
    RequestProcessor *request_processor = malloc(sizeof(RequestProcessor));
    request_processor->n_requests = 0;
    request_processor->worker = create_queue_worker();
    pthread_create(&request_processor->worker->thread, NULL, process_queue, request_processor->worker);
    return request_processor;
}

void destroy_request_processor(RequestProcessor **rp) {
    if (DEBUG)
        printf("Destroying request processor\n");
    (*rp)->worker->run_worker = 0;
    pthread_join((*rp)->worker->thread, NULL);
    destroy_queue_worker(&(*rp)->worker);
    free(*rp);
    *rp = NULL;
}

void add_request(RequestProcessor *rp, int room, int access_level, int door, int *data_return) {
    if (DEBUG)
        printf("Adding request room %d, door %d, %d access\n", room, door, access_level);
    Request *request = (Request *) malloc(sizeof(Request));
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
    if (DEBUG)
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

unsigned int random_between(unsigned int min, unsigned int max) {
    return (int) (rand() % (max + 1 - min)) + min;
}

int wait_for_n_seconds(char *str, int max_wait_time, int max_answer_time) {
    int max_time = max_wait_time;
    int control_room_mock_answer = random_between(0, max_answer_time);
    while (max_time > 0) {
        if (control_room_mock_answer == 0) {
            return random_between(0, 1);
        }
        if (DEBUG)
            printf("Waiting for %s answer: %d\n", str, control_room_mock_answer);
        sleep(1);
        control_room_mock_answer--;
        max_time--;
    }
    return -1;
}

void process_request(Request *request) {
    if (request->room == Inner) {
        // 2. get answer from control room
        // sleep for 1 second at a time to wait for control room
        // set an interval to check for control room answer (don't sleep for N)
        // if control room answer is received, break
        int control_room_approval = wait_for_n_seconds("control room", 5, 6);
        *request->response = control_room_approval;
    } else {
        // get door states
        // if door is open, wait for it to close
        // if it doesnt close in N seconds, deny
        int control_room_approval = wait_for_n_seconds("door", 5, 4);
        *request->response = control_room_approval;
    }
}