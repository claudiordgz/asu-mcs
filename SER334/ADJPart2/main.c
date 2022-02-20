#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "RequestProcessor.h"

void* simulate_door_open(void* door_id);
RequestProcessor *rp;

int main(void) {
  // scanf get action
  rp = create_request_processor();
  int number_of_requests = 30;
  pthread_t threads[number_of_requests];

  for (int i = 0; i < number_of_requests; i++) {
      printf("Request %d: door to open\n", i);
      pthread_create(&threads[i], NULL, &simulate_door_open, (void*)i);
  }
  for (int i = 0; i < number_of_requests; i++)
      pthread_join(threads[i], NULL);   
  destroy_request_processor(&rp);
  return 0;
}

void* simulate_door_open(void* thread_n) {
  int t_id = (int)thread_n;
  int data = rand() % 100;
  int* result = (int*)malloc(sizeof(int));
  *result = -1;

  int ms = (rand() % 100) * 1000;
  nanosleep((struct timespec[]){{0, ms*1000000}}, NULL);
  
  printf("User #%d: Wants to process data=%d and store it at %p.\n", (int)t_id, data, result);
  
  //make request to balance to complete job and wait for it's completion.
  add_request(rp, 0, 1, 0, result);
  while(*result == -1); 
  
  printf("User #%d: Received result from data=%d as result=%d.\n", (int)t_id, data, *result);
  
  free(result);
  
  pthread_exit(NULL);
}