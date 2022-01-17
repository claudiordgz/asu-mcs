#include <stdio.h>

struct Course {
    int sln;
    char title[256];
    char instructor[250];
    int time;
};

typedef struct Course Course;
Course ser222;
int main() {
    ser222.sln = 1774;
    strcpy(ser222.title, "Data Structures & Algorithms");
    strcat(ser222.instructor, "Acuna, Ruben");

    printf("Memory allocated for this structure is %d", sizeof(ser222));
    return 0;
}
