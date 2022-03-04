#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 1
#define TYPE_SENSOR 0

struct sensor_packet {
    int id;
};

struct sensor_state {
    pthread_mutex_t lock;
    struct sensor_packet *current;
};

struct thread_data {
    pthread_t thread;
    struct sensor_state *data_source;
    int *exit_signal;
    int interval;
};


void* datastream_runner(void* info);

int main ()
{
    int a = 10;
    int b = 20;
    int c = a + b;
    return 0;
}

void* datastream_runner(void* info) {
    struct thread_data *thr_data = (struct thread_data*) info;
    struct sensor_state *sensor = thr_data->data_source;
    int* ext = thr_data->exit_signal;
    int interval = thr_data->interval;
    struct sensor_packet* data[MAX];
    struct sensor_packet data_packet[MAX];
    int used = 0;
    
    while(ext) {
        while(used < MAX) {
            data[used] = malloc(sizeof(struct sensor_packet));
            pthread_mutex_lock(&sensor->lock);
                *data[used++] = *sensor->current;
            pthread_mutex_unlock(&sensor->lock);
            wait(internal);
        }
        
        for(int i = 0; i < MAX; i++) //consolidate into contiguous data
            data_packet[i] = *data[i];
        
        transmit(data_packet, sizeof(struct sensor_packet) * MAX, TYPE_SENSOR);
        used = 0;
    }
}
