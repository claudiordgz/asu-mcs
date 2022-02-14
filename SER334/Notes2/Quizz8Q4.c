#include <pthread.h>
#include <stdlib.h>

struct sem_node {
    int pid;
    struct sem_node* next;
};
struct semaphore {
    int count;
    pthread_mutex_t lock;
    struct sem_node* head;
};
typedef struct semaphore semaphore;
struct semaphore* create_semaphore(int n) {
    struct semaphore* sem = (struct semaphore*)malloc(sizeof(struct semaphore));
    sem->count = n;
    pthread_mutex_init(&sem->lock, NULL);
    sem->head = NULL;
    return sem;
}

void add_to_list(semaphore* s);
void remove_from_list(semaphore* s, int pid);

void wait(semaphore* s) {
    pthread_mutex_lock(&s->lock);
    s->count--;
    if (s->count < 0) {
        // add process to list
        add_to_list(s);
        block();
    }
    pthread_mutex_unlock(&s->lock);
}
void signal(semaphore* s) {
    pthread_mutex_lock(&s->lock);
    if (s->value <= 0) {
        // remove from list of processes
        int pid = get_pid();
        remove_from_list(s, pid);
        wakeup(pid);
    }
    pthread_mutex_unlock(&s->lock);
}

void add_to_list(semaphore* s) {
    struct sem_node* new_node = (struct sem_node*)malloc(sizeof(struct sem_node));
    new_node->pid = pthread_self();
    new_node->next = s->head;
    if (s->head == NULL) {
        s->head = new_node;
    } else {
        while (s->head->next != NULL) {
            s->head = s->head->next;
        }
        s->head->next = new_node;
    }
}

void remove_from_list(semaphore* s, int pid) {
    struct sem_node* temp = s->head;
    if (temp->pid == pid) {
        s->head = temp->next;
        free(temp);
    } else {
        while (temp->next != NULL) {
            if (temp->next->pid == pid) {
                struct sem_node* temp2 = temp->next;
                temp->next = temp2->next;
                free(temp2);
                break;
            }
            temp = temp->next;
        }
    }
}
