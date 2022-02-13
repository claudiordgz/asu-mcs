/**
 * Instance Definition for LoadBalancer Mutex Exercise
 *
 * Completion time: 4h
 * @author Claudio Rodriguez Rodriguez
 * @version 02/12/2022
 */
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "InstanceHost.h"

#define DEBUG 0

struct node {
    pthread_t instance_id;
    struct job_node *batch;
    struct node *next;
};
typedef struct node node;

struct host {
    pthread_mutex_t lock;
    node *instances_head;
    node *instances_tail;
    int instance_count;
};

void remove_batch(host* h, pthread_t instance_id);
void add_new_batch(host* h, struct job_node *batch);

host* host_create() {
    host *h = (host*)malloc(sizeof(host));
    pthread_mutex_init(&h->lock, NULL);
    h->instances_head = NULL;
    h->instances_tail = NULL;
    h->instance_count = 0;
    return h;
}

void host_destroy(host** h) {
    if (DEBUG)
        printf("Host: Destroying host.\n");

    node *current = NULL;
    pthread_mutex_lock(&(*h)->lock);
        current = (*h)->instances_head;
    pthread_mutex_unlock(&(*h)->lock);

    while (current != NULL) {
        if (DEBUG)
            printf("Terminating instance %lu.\n", current->instance_id);
        pthread_join(current->instance_id, NULL);
        
        pthread_mutex_lock(&(*h)->lock);
            remove_batch(*h, current->instance_id);
            current = (*h)->instances_head;
        pthread_mutex_unlock(&(*h)->lock);
    }

    (*h)->instances_head = NULL;
    (*h)->instances_tail = NULL;
    (*h)->instance_count = 0;
    pthread_mutex_destroy(&(*h)->lock);
    free(*h);
    *h = NULL;
}

void *host_run_instance(void *data) {
    struct job_node* batch = (struct job_node*)data;
    struct job_node* itr = batch;
    while (itr != NULL) {
        *(itr->data_result) = itr->data * itr->data;
        itr = itr->next;
    }
    pthread_exit(0);
}

void host_request_instance(host* h, struct job_node* batch) {
    if (DEBUG)
        printf("LoadBalancer: Received batch and spinning up new instance.\n");
    int count = 0;
    node *current = NULL;

    pthread_mutex_lock(&h->lock);

        add_new_batch(h, batch);
        current = h->instances_tail;
        count = h->instance_count;

        if (DEBUG)
            printf("Total instances: %d\n", count);

        pthread_create(&(current->instance_id), NULL, host_run_instance, (void*)batch);

    pthread_mutex_unlock(&h->lock);


}

void add_new_batch(host* h, struct job_node *batch) {
    node *new_node = (node*)malloc(sizeof(node));
    new_node->batch = batch;
    new_node->next = NULL;
    if (h->instances_tail == NULL) {
        h->instances_head = new_node;
    } else {
        h->instances_tail->next = new_node;
    }
    h->instances_tail = new_node;
    h->instance_count++;
}

void remove_batch(host* h, pthread_t instance_id) {
    node *itr = h->instances_head;
    node *prev = NULL;
    while (itr != NULL) {
        if (itr->instance_id == instance_id) {
            if (prev == NULL) {
                if (itr->instance_id == h->instances_tail->instance_id) {
                    h->instances_tail = itr->next;
                }
                h->instances_head = itr->next;
            } else {
                if (itr->instance_id == h->instances_tail->instance_id) {
                    if (itr->next == NULL) {
                        h->instances_tail = prev;
                    } else {
                        h->instances_tail = itr->next;
                    }
                }
                prev->next = itr->next;
            }
            free(itr);
            h->instance_count--;
            return;
        }
        prev = itr;
        itr = itr->next;
    }
}
