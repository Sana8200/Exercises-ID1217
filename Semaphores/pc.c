/* a simple producer/consumer using semaphores and threads

   usage on MacOS:
     gcc thisfile.c -lpthread 
     a.out numIters

*/
#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#define SHARED 1

void *Producer(void *);  /* the two threads */
void *Consumer(void *);

sem_t *empty, *full; /* the global semaphores */
const char sem_name_empty[] = "./empty";
const char sem_name_full[] = "./full";
int data;             /* shared buffer         */
int numIters;

/* main() -- read command line and create threads, then
             print result when the threads have quit */

int main(int argc, char *argv[]) {
  /* thread ids and attributes */
  pthread_t pid, cid;  

  numIters = atoi(argv[1]);
  empty = sem_open(sem_name_empty, O_CREAT, 0644, 1);
  full = sem_open(sem_name_full, O_CREAT, 0644, 0);
  if(empty == (void*)-1){
   perror("sem_open failure");
   exit(1);
 }
  printf("main started\n");
  pthread_create(&pid, NULL, Producer, NULL);
  pthread_create(&cid, NULL, Consumer, NULL);
  pthread_join(pid, NULL);
  pthread_join(cid, NULL);
  sem_close(empty);
  sem_close(full);
  printf("main done\n");
}

/* deposit 1, ..., numIters into the data buffer */
void *Producer(void *arg) {
  int produced;
  printf("Producer created\n");
  for (produced = 0; produced < numIters; produced++) {
    sem_wait(empty);
    data = produced;
    sem_post(full);
  }
  return NULL;
}

/* fetch numIters items from the buffer and sum them */
void *Consumer(void *arg) {
  int total = 0, consumed;
  printf("Consumer created\n");
  for (consumed = 0; consumed < numIters; consumed++) {
    sem_wait(full);
    total = total+data;
    sem_post(empty);
  }
  printf("for %d iterations, the total is %d\n", numIters, total);
  return NULL;
}
