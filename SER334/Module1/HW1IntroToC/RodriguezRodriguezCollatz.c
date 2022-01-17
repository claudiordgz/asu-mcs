/**
* SER334: Operating Systems & System Programming
* Introduction to C Programming I
* Exercise 2: Collatz
* Collatz conjecture checker.
*
* Completion time: 30m
*
* @author Claudio Rodriguez Rodriguez
* @version 01/16/2022 0.0.1
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>

int isEven(int number) {
    return number % 2 == 0;
}

void termination(int collatzNumber) {
    int iterations = 0;
    int n = collatzNumber;
    while (n != 1) {
        if (isEven(n)) {
            n = n / 2;
        } else {
            n = 3*n + 1;
        }
        iterations += 1;
    }
    printf("Reached 1 on %d iterations\n", iterations);
}

int main(void)
{
    int collatzNumber = 0;
    printf("Collatz Conjecture.\n");
    while (collatzNumber < 1) {
        printf("Enter a number:\n");
        scanf("%d", &collatzNumber);
        if (collatzNumber < 1) {
            printf("Only positive integers are allowed.\n");
        }
    }
    termination(collatzNumber);
    return 0;
}