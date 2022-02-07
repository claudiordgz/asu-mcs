#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

void* display(void* data) {
    printf("From the function, the thread id = %ld\n", pthread_self());
    pthread_exit(0);
}

int main() {
    pid_t before_pid, after_pid;
    pthread_t tid1, tid2;
    before_pid = getpid();
    pid_t pid;
    printf("%d\n", before_pid);
    pid = fork();
    printf("pid = %d\n", pid);

    pthread_create(&tid1, NULL, display, NULL);

    if (pid == 0) {
        pid = fork();
        printf("pid = %d\n", pid);
        if(pid == 0) {
            pid = fork();
            printf("pid = %d\n", pid);
        }
    }
    pthread_create(&tid2, NULL, display, NULL);
    
    after_pid = getpid();
    printf("after pid = %d\n", pid);

    return 1;
}