#include <stdio.h>
#include <malloc.h>

struct point_2d {
    int x;
    int y;
};

typedef struct point_2d point_2d;

int main() {
    point_2d *data[5];
    int i = 0;
    for (i = 0; i != 5; ++i) {
        data[i] = (point_2d *) malloc(sizeof(point_2d));
        data[i]->x = i;
        data[i]->y = i * i;
    }
    for (i = 0; i != 5; ++i) {
        printf("Point #%d:%d, %d", i, data[i]->x, data[i]->y);
        free(data[i]);
    }
    return 0;
}
