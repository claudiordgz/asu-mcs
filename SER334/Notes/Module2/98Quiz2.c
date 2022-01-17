#include <stdio.h>

char* to_upper_case(char* original) {
    // char capstr[255];
    char * capstr;
    capstr = (char*) malloc(255 *(sizeof(char)));
    int i;
    for (i = 0; original[i] != '\0'; ++i) {
        if (original[i] >= 'a' && original[i] <= 'z') {
            capstr[i] = original[i] - (char)'a' + (char)'A';
        } else {
            capstr[i] = original[i];
        }
    }
    capstr[i] = '\0';
    return capstr;
}

int main() {
    char* temp = to_upper_case("the c programming language");
    printf("%s", temp);
    free(temp);
    return 0;
}