#include <stdio.h>
#include <stdlib.h>


int win()
{
    printf("Great Job\n");
    exit(0);
}

int main()
{
    char buffer[40];
    unsigned long long target;

    scanf("%s", buffer);
    printf("You sent: %s\n", buffer);
    exit(1);
}
