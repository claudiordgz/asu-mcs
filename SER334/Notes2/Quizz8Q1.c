#include <pthread.h>
#include <stdio.h>
pthread_mutex_t lock_a, lock_b, lock_c;

void* runner1(void* param) {
    pthread_mutex_lock(&lock_a);
    printf("X");
    pthread_mutex_unlock(&lock_b);
    pthread_mutex_unlock(&lock_a);
    pthread_exit(0);
}
void* runner2(void* param) {
    pthread_mutex_lock(&lock_a);
    printf("Y");
    pthread_mutex_unlock(&lock_c);
    pthread_mutex_unlock(&lock_a);

    pthread_exit(0);
}
void* runner3(void* param) {
    pthread_mutex_lock(&lock_b);
    printf("Z");
    pthread_mutex_unlock(&lock_b);
    pthread_exit(0);
}
int main(int argc, char* argv[]) {
    pthread_t tids[3];
    pthread_mutex_init(&lock_a, NULL);
    pthread_mutex_init(&lock_b, NULL);
    pthread_mutex_init(&lock_c, NULL);
    pthread_mutex_lock(&lock_a);
    pthread_mutex_lock(&lock_b);
    pthread_mutex_lock(&lock_c);
    pthread_create(&tids[0], NULL, runner1, NULL);
    pthread_create(&tids[1], NULL, runner2, NULL);
    pthread_create(&tids[2], NULL, runner3, NULL);
    pthread_join(tids[0], NULL);
    pthread_join(tids[1], NULL);
    pthread_join(tids[2], NULL);
}