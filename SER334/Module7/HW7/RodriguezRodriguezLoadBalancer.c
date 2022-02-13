/**
 * Load Balancer Definition for LoadBalancer Mutex Exercise
 *
 * Completion time: 4h
 * @author Claudio Rodriguez Rodriguez
 * @version 02/12/2022
 */
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "LoadBalancer.h"
#include "InstanceHost.h"

#define DEBUG 0

typedef struct job_node job_node;
typedef struct node node;

struct node {
    job_node *_node;
    struct node *next;
};

struct balancer {
    pthread_mutex_t lock;
    job_node *head;
    job_node *tail;
    node *all_jobs;
    host *host;
    int job_count;
    int batch_size;
};

void balancer_list_add_job(balancer* lb, struct job_node *new_job);
void add_to_tail(balancer* lb, struct job_node *new_job);
void balancer_add_to_all_jobs(balancer* lb, struct job_node *new_job);

balancer* balancer_create(int batch_size) {
    if (DEBUG)
        printf("LoadBalancer: Creating new balancer.\n");
    balancer *lb = (balancer*)malloc(sizeof(balancer));
    pthread_mutex_init(&lb->lock, NULL);
    lb->head = NULL;
    lb->batch_size = batch_size;
    lb->all_jobs = NULL;
    
    lb->host = host_create();
    if (DEBUG)
        printf("LoadBalancer: Created new balancer.\n");
    return lb;
}

void balancer_destroy(balancer** lb) {
    if (DEBUG)
        printf("LoadBalancer: Destroying balancer.\n");
    while ((*lb)->job_count > 0) {
        pthread_mutex_lock(&(*lb)->lock);
            balancer_list_add_job(*lb, NULL);
        pthread_mutex_unlock(&(*lb)->lock);
    }

    struct node *current = (*lb)->all_jobs;
    while (current != NULL) {
        struct node *next = current->next;
        free(current);
        current = next;
    }
    (*lb)->all_jobs = NULL;
    host_destroy(&(*lb)->host);
    (*lb)->host = NULL;
    pthread_mutex_destroy(&(*lb)->lock);
    free((*lb));
    *lb = NULL;
}


void balancer_add_job(balancer* lb, int user_id, int data, int* data_return) {
    if (lb->host == NULL) {
        printf("ERROR: Host can't be NULL");
        pthread_exit((void*)1);
    }

    printf("LoadBalancer: Received new job from user #%d to process data=%d and store it at %p\n", user_id, data, data_return);

    struct job_node *new_job = (struct job_node*)malloc(sizeof(struct job_node));
    new_job->user_id = user_id;
    new_job->data = data;
    new_job->data_result = data_return;
    new_job->next = NULL;

    pthread_mutex_lock(&lb->lock);
        balancer_list_add_job(lb, new_job);
        balancer_add_to_all_jobs(lb, new_job);
    pthread_mutex_unlock(&lb->lock);
}

void balancer_list_add_job(balancer* lb, struct job_node *new_job) {
    if (new_job == NULL) {
        host_request_instance(lb->host, lb->head);
        lb->head = NULL;
        lb->tail = NULL;
        lb->job_count = 0;
    } else if (lb->head == NULL) {
        lb->head = new_job;
        lb->tail = new_job;
        lb->job_count = 1;
    } else if (lb->job_count == lb->batch_size) {
        // do not allow jobs to reach 0 to prevent early termination
        host_request_instance(lb->host, lb->head);
        lb->head = new_job;
        lb->tail = new_job;
        lb->job_count = 1;
    } else {
        add_to_tail(lb, new_job);
        lb->job_count++;
    }
    if (DEBUG)
        printf("LoadBalancer: JOB QUEUE SIZE %d.\n", lb->job_count);
}

void add_to_tail(balancer* lb, struct job_node *new_job) {
    job_node *curr = lb->tail;
    curr->next = new_job;
    lb->tail = curr->next;
}

void balancer_add_to_all_jobs(balancer* lb, struct job_node *new_job) {
    node *new_node = (node*)malloc(sizeof(node));
    new_node->_node = new_job;
    new_node->next = NULL;

    if (lb->all_jobs == NULL) {
        lb->all_jobs = new_node;
    } else {
        node *curr = lb->all_jobs;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = new_node;
    }
}