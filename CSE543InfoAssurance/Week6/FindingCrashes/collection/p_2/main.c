#include <stdio.h>
#include <stdlib.h>

typedef void (*funcptr_t)();


int win()
{
    printf("Great Job\n");
    exit(0);
}

int main()
{
    int n = 0;
    unsigned long long target;

    printf("This is your third challenge. The bread is getting harder and harder!\n");
    scanf("%d", &n);
    scanf("%llu", &target);
    if (n == 0xc0decafe) {
        funcptr_t funcptr = (funcptr_t)target;
        funcptr();
    }
}
