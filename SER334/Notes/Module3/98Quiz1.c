#include <stdio.h>


// Create a macro called PI with precision of two decimal places.
#define PI 3.14

// Create a macro called AREA_OF_SPHERE that takes in value radius and computes
// the surface area of a sphere.
// Hint: surface area of a sphere is four times pi times the radius squared.
#define AREA_OF_SPHERE(radius) (4*PI*radius*radius)

// Create a macro called VOLUME_OF_CONE that takes in values radius, height and
// computes the volume of a cone.
// Hint: volume of a cone is one-third times pi times radius squared * height.
#define VOLUME_OF_CONE(radius,height) (((double)1/(double)3)*PI*radius*radius*height)


int main() {
    int radius = 3;
    int height = 5;
    float area = AREA_OF_SPHERE(radius);
    float volume = VOLUME_OF_CONE (radius, height);

    printf("%f\n", area);
    printf("%f\n", volume);
    return 0;
}