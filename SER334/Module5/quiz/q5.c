#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid1, pid2;

    printf("A ");

    pid1 = fork();

    if (pid1 < 0) {
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid1 == 0) {
        printf("B ");
        printf("C ");
    }
    else {
        wait(NULL);
        printf("D ");
    }
    return 0;
}