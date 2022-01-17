#include <stdio.h>
#include <stdlib.h>

struct custom_node {
    int data;
    struct custom_node* next;
};

void remove_first(struct custom_node** head) {
    struct custom_node* tmp;
    tmp = *head;
    *head = (*head)->next;
    free(tmp);
}

struct custom_node* list;


void insert_node(struct custom_node* new_node) {
    if (list != NULL){
        struct custom_node* iterator = list;
        while (1) {
            if (new_node->data > iterator->data) {
                if (iterator->next != NULL) {
                    iterator = iterator->next;
                } else {
                    iterator->next = new_node;
                    break;
                }
            } else {
                new_node->next = iterator;
                iterator->next = new_node;
                break;
            }
        }
    }
}

typedef struct custom_node custom_node;
int main() {
    list = (struct custom_node*) malloc(sizeof(struct custom_node));

    custom_node* n1 = (custom_node*) malloc(sizeof(custom_node));
    custom_node* n2 = (custom_node*) malloc(sizeof(custom_node));
    custom_node* n3 = (custom_node*) malloc(sizeof(custom_node));
    custom_node* n4 = (custom_node*) malloc(sizeof(custom_node));
    custom_node* n5 = (custom_node*) malloc(sizeof(custom_node));
    list = n1;
    n1-> data = 1;
    n1-> next = n2;
    n2-> data = 2;
    n2-> next = n3;
    n3-> data = 3;
    n3-> next = n5;
    n4-> data = 4;

    n5-> data = 5;
    insert_node(n4);
    printf("1 %d\n", n4->next->data);
//    custom_node* n2 = (custom_node*) malloc(sizeof(custom_node));
//    n2 -> data = 108;
//    n1 -> next = n2;
//    printf("2 %d\n", n2->data);
//    remove_first(&n1);
//    printf("3 %d\n", n1->data);
//    printf("4 %d\n", n2->data);
    return 0;
}