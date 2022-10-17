#include <stdio.h>


int main()
{
    printf("This is your second challenge. The bread getting harder!\n");
    unsigned long long key = getchar();
    if (key == 0x53) {
        puts((char*)(key));
    }
}
