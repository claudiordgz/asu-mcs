#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

typedef struct node node;

struct node {
    void* element;
    node* next;
};

struct list {
    node* head;
    int size;
};
//(a) Implement list_create. This function does not take any parameters but will return
//    a new list collection.
list* list_create() {
    list* newList = (list*)malloc(sizeof(list));
    if (newList == NULL) {
        printf("Failed to create list!");
        exit(1);
    } else {
        newList->head = NULL;
        newList->size = 0;

        return newList;
    }
}
//(a) Implement list_destroy. This function should take a list as a parameter and clean up
//    its memory allocation(s).
void list_destroy(list** lst) {
    node *iter = (*lst)->head;

    while (iter != NULL) {
        node *next = iter->next;
        free(iter);
        iter = next;
    }
    free(*lst);
    *lst = NULL;
}

//(b) Implement list_add. This function should take a list, and an element, as parameters
//    and append the element to the end of the list.
void list_add(list* lst, void* element) {
    node* new_node = (node*)malloc(sizeof(node));
    new_node->element = element;
    new_node->next = NULL;

    if (lst->head == NULL) {
        lst->head = new_node;
    } else {
        node *iter = lst->head;
        while (iter->next != NULL) {
            iter = iter->next;
        }
        iter->next = new_node;
    }
    lst->size += 1;
}

//(c) Implement list_size. This function should return the number of elements in the list.
int list_size(list* lst) {
    return lst->size;
}
