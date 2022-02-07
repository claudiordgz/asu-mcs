#include <pthread.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_THREADS 2

struct data_for_threads {
    char *word;
    int len;
    int thread_id;
};

char ch_arr[12][10] = { "First", "Second", "Third", "Fourth", "Fifth", "Sixth", "Seventh", "Eighth", "Ninth", "Tenth", "Eleventh", "Twelfth" };
void* display(void* data) {
    struct data_for_threads *d = (struct data_for_threads*)data;
    char *ordinal = ch_arr[d->thread_id];
    int chunk_len = MAX_THREADS > d->len ? 1 : (int)d->len / MAX_THREADS;
    int start_index = d->thread_id * chunk_len;
    char *start = d->word + start_index;
    char thread_string[chunk_len + 1]; 
    strncpy(thread_string, start, chunk_len);
    printf("%s Thread: %s\n", ordinal, thread_string);
    
    pthread_exit(0);
}

int main() {
    char* name = "Wonderbread!";
    int LEN = 12;
    int threads = MAX_THREADS > LEN ? (int)LEN : MAX_THREADS;   
    //TODO!
    int i = 0;
    struct data_for_threads tdata[threads];
    pthread_t tids[threads];
    for (i = 0; i < threads; i++) {
        tdata[i].word = name;
        tdata[i].len = LEN;
        tdata[i].thread_id = i;
        pthread_create(&tids[i], NULL, display, &tdata[i]);
    }

    for (i = 0; i < threads; i++) {
        pthread_join(tids[i], NULL);
    }

    return 1;
}