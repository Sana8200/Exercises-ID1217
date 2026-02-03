#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/times.h>
#include <unistd.h>
#define MAXTHREADS 8
void *tf (void *);
int n = MAXTHREADS, results[MAXTHREADS], total = 0;
int main (int argc, char *argv[]) {
  pthread_t tid[MAXTHREADS];
  long i;
  n = (argc > 1)? atoi(argv[1]) : MAXTHREADS;
  for (i = 0; i < n; i++) pthread_create (&tid[i], NULL, tf, (void *) i);
  for (i = 0; i < n; i++) {pthread_join (tid[i], NULL); total += results[i];}
  printf ("Total: %d\n", total);
  return 1;
}
void *tf (void *num) {
  int i;
  long mynum = (long)num;
  struct tms buffer;
  clock_t time = times (&buffer);
  for (i = 0; i < 5; i++)
      printf ("I am thread %ld of %d\n", mynum, n); sleep (rand() % 2);
  time = times (&buffer) - time;
  results[mynum] = (int) time;
  return (NULL);
}

