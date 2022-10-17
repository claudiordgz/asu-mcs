#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int win()
{
    printf("Great Job\n");
    exit(0);
}


int mycpy(char* input)
{
    char buffer[40];
    strcpy(buffer, input);
    return strlen(buffer);
}


int main()
{
    char buffer[256];
    unsigned long long target;

    scanf("%256s", buffer);
    mycpy(buffer);
    exit(1);
}
