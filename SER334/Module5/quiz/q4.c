#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
int main() {
       pid_t pid, pid1;
       /* fork a child process */
    printf("A1\n");
       pid = fork();
    printf("A2\n");
       if (pid < 0) { /* error occurred */
              fprintf(stderr, "Fork Failed");
              return 1;
       }
       else if (pid == 0) {
              pid1 = getpid();
              printf("child: pid = %d A\n", pid); /* A */
              printf("child: pid1 = %d B\n", pid1); /* B */
           printf("B\n");
           printf("C\n");
       }
       else {
              pid1 = getpid();
              printf("parent: pid = %d C\n", pid); /* C */
              printf("parent: pid1 = %d D\n", pid1); /* D */
           printf("D\n");
              wait(NULL);
       }
       return 0;
}