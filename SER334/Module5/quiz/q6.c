#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
int main() {
    pid_t pid1, pid2;
    //TODO: Implement code starting here!
    pid1 = fork();
    if (pid1 < 0) { //first fork failed
        fprintf(stderr, "Fork Failed P1");
        return 1;
    }
    else if (pid1 == 0) { //first fork worked and we are in child
        printf("Process 1 pid: %d\n", getpid());
        pid2 = fork();
        if (pid2 < 0) { //first fork failed
            fprintf(stderr, "Fork Failed P2");
            return 1;
        }
        else if (pid2 == 0) { //first fork worked and we are in child
            printf("Process 2 pid: %d\n", getpid());
            pid2 = fork();
            if (pid2 < 0) { //first fork failed
                fprintf(stderr, "Fork Failed P3");
                return 1;
            }
            else if (pid2 == 0) { //first fork worked and we are in child
                printf("Process 3 pid: %d\n", getpid());
            }
            else {
                wait(NULL);
            }
        }
        else {
            wait(NULL);
        }
    }
    else {
        wait(NULL);
    }


    return 0;
}