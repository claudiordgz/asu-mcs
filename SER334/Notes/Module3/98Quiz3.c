#include <stdio.h>
#include <math.h>

#define DIST(deltax, deltay)  (deltax*deltax + deltay*deltay)

int main() {
    float x1 = 2.0f, x2 = 5.0f;
    float y1 = 5.0f, y2 = 10.0f;
    float dx = x2-x1;
    float dy = y2-y1;

    printf("%f.\n", dx);
    printf("%f.\n", dy);
    printf("Distance with margin is %f.\n", DIST(dx++, dy++));
    printf("%f.\n", dx);
    printf("%f.\n", dy);
    return 0;
}