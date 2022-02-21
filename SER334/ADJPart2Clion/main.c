#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "RequestProcessor.h"

void *simulate_door_open(void *door_id);

RequestProcessor *rp;

int main(void) {
    srand(time(NULL));
    // scanf get action
    rp = create_request_processor();
    int number_of_requests = 30;
    pthread_t threads[number_of_requests];

    for (int i = 0; i < number_of_requests; i++) {
        pthread_create(&threads[i], NULL, &simulate_door_open, (void *) i);
    }
    for (int i = 0; i < number_of_requests; i++)
        pthread_join(threads[i], NULL);
    destroy_request_processor(&rp);
    printf("Exiting");
    return 0;
}

void *simulate_door_open(void *thread_n) {
    int t_id = (int) thread_n;

    int *result = (int *) malloc(sizeof(int));
    *result = -99;

    int s = random_between(0, 10);
    sleep(s);
    int room = random_between(0, 1);
    int access_level = random_between(0, 10);
    int door = random_between(0, 1);

    printf("Request %d: door %s, room %s, access level %d\n", t_id, door == 0 ? "A" : "B",
           room == 0 ? "inner" : "outer", access_level);

    //make request to balance to complete job and wait for it's completion.

    add_request(rp, room, access_level, door, result);
    while (*result == -99);

    int r = *result;
    char *str = r == -1 ? "Cancelled" : r == 0 ? "Denied" : "Approved";
    printf("Request %d: RESULT [%s]\n", t_id, str);

    free(result);

    pthread_exit(NULL);
}