#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

//represents a mutex
struct mutex_lock {
    //TODO
    int lock;
};

//creates and returns a new mutex, initally unlocked.
struct mutex_lock* mutex_create(void) {
    //TODO
    struct mutex_lock* m = (struct mutex_lock*)malloc(sizeof(struct mutex_lock));
    m->lock = 0;
    return m;
}

//locks mutex
void mutex_acquire(struct mutex_lock* m) {
    //TODO
    while(test_and_set(&m->lock));
}

//unlockes mutex
void mutex_release(struct mutex_lock* m) {
    //TODO
    m->lock = 0;
}

int main() {
  int choice = 0;
  struct mutex_lock *m;
  m = mutex_create();

  while (1) {
    printf("Enter your choice\n");
    printf("1. Mutex acquire\n");
    printf("2. Mutex release\n");
    printf("3. Exit\n");
    scanf("%d", & choice);

    switch (choice) {
    case 1:
      mutex_acquire(m);
      break;

    case 2:
      mutex_release(m);
      break;

    case 3:
      exit(0);

    default:
      printf("Invalid choice. Please try again\n");
    }
  }
    free(m);
  return 0;
}