#include <stdio.h> //for malloc#
#include <string.h>
#include <stdlib.h>
//a struct to contain counter information
struct counter {
    char name[256];
    int count;
};

typedef struct counter counter;

//function to create counters
struct counter* create_counter(char* name) {
    counter* new_counter = (counter*) malloc(sizeof(counter));
    if (new_counter == NULL) {
        printf("Failed to create counter.\n");
        exit(1);
    }

    strcpy(new_counter->name, name);
    new_counter->count = 0;
    return new_counter;
}

//function to increment a counter
void increment(counter* c) {
    c->count += 1;
}

//function to get a counter's tally
int tally(counter* c) {
    return c->count;
}

//function to display a counter's tally
void display(counter* c) {
    printf("%s shows %d.\n", c->name, c->count);
}

//SAMPLE USAGE
int main() {
    struct counter* c1 = create_counter("First");
    struct counter* c2 = c1;
    increment(c1);
    increment(c2);
    display(c1);  //output: First shows 2.
    display(c2);  //output: First shows 2.
    return 0;
}