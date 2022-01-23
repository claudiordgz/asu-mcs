#include <stdio.h>
#include "LinkedList.h"

int main() {
    list* lst;
    lst = list_create();

    list_add(lst, 10);
    list_add(lst, 15);
    list_add(lst, 20);

    printf("Size: %d\n", list_size(lst));

    list_destroy(&lst);
    return 0;
}