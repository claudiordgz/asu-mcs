/**
* SER334: Operating Systems & System Programming
* Introduction to C Programming I
* Exercise 1: Computation
* Allows calculating total volume of multiple cylinders
* Prompts for:
*  - Total Number of Cylinders
*  - Radius and Height for each Cylinder
* Displays:
*  - Total volume of all cylinders
*
* Completion time: 1 hour
*
* @author Claudio Rodriguez Rodriguez, Acuna
* @version 01/16/2022 1.0.0
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>

int main(void)
{
    int totalCylinders = 0;
    int i = 0;
    float totalVolume = 0;

    //Add a prompt for the number of cylinders to sum. Make sure to use the control
    //symbol for integers. [2 points]
    printf("Welcome to the Cylinder Volume calculator.\n");
    printf("Enter number of cylinders to use:\n");
    scanf("%d", &totalCylinders);
    printf("Calculating total volume for %d cylinders.\n", totalCylinders);

    //Create a loop based on the number of cylinders the user enters. [2 points]
    for (i = 0; i != totalCylinders; ++i) {
        {
            //    Within the loop, prompt for height and radius parameters (allow floating
            //    point numbers), and calculate the volume for that particular cylinder.
            //    [4 points]
            int cylinderNumber = i + 1;
            float height = 0;
            float radius = 0;
            float volumeOfCylinder = 0;
            printf("Enter height of cylinder:\n");
            scanf("%f", &height);
            printf("Enter radius of cylinder:\n");
            scanf("%f", &radius);
            volumeOfCylinder = height * radius;
            printf("Volume of cylinder %d is %f\n", cylinderNumber, volumeOfCylinder);
            totalVolume += volumeOfCylinder;
        }
    }

    //Display the total volume sum back to the user. Make sure to use the right control
    //symbol. [2 points]
    printf("Volume of all Cylinders is %f\n", totalVolume);

    return 0;
}