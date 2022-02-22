#include <assert.h>
#include "DataLoader.h"

#define DEBUG 0

void display_scenario(struct test_scenario* s);

struct test_scenario* load_test_data(char* filename) {
    FILE *file = fopen(filename,"r");
    if (!file)
    {
        printf("Cannot open file %s.\n", filename);
        return NULL;
    }  
    struct test_scenario* scenario = (struct test_scenario *)malloc(sizeof(struct test_scenario));
    fseek(file, 0, SEEK_SET);
    int error = 1;
    error = fscanf(file, "%d\n", &scenario->page_count);
    assert(error == 1);
    error = fscanf(file, "%d\n", &scenario->frame_count);
    assert(error == 1);
    error = fscanf(file, "%d\n", &scenario->refstr_len);
    assert(error == 1);
    int i = 0;
    for (i = 0; i != scenario->refstr_len; ++i) {
        error = fscanf(file, "%d\n", &scenario->refstr[i]);
        assert(error == 1);
    }
    scenario->refstr[i] = -1;

    if (DEBUG) {
        display_scenario(scenario);
    }

    fclose(file);
    return scenario;    
}

void display_scenario(struct test_scenario* s) {
    printf("Page count: %d\n", s->page_count);
    printf("Frame count: %d\n", s->frame_count);
    printf("Reference string length: %d\n", s->refstr_len);
    int i = 0;
    for (i = 0; i != s->refstr_len; ++i) {
        printf("%d ", s->refstr[i]);
    }
    printf("\n");
}