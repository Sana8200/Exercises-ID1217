#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 5		/* default number of threads */
/* shared variables */
double total;
pthread_mutex_t lock;
void *Counter (void *null) {
  int i;
  double *result = (double *) calloc (1, sizeof (double));
  for (i = 0; i < 1000000; i++) *result = *result + (double) (random ()%100);
  pthread_mutex_lock (&lock);
  total += *result;
  pthread_mutex_unlock (&lock);
  pthread_exit ((void *) result);
}
int main (int argc, char *argv[]) {
  int n = NUM_THREADS;
  double *result;
  result = (double*)calloc(1, sizeof(double));
  pthread_t thread[NUM_THREADS];
  int t;
  if (argc > 1) n = atoi (argv[1]);
  if (n > NUM_THREADS || n < 1) n = NUM_THREADS;
  for (t = 0; t < n; t++) pthread_create (&thread[t], NULL, Counter, NULL);
  for (t = 0; t < n; t++) {
      pthread_join (thread[t], (void*) &result);
      printf ("Completed join with thread %d. Result =%f\n", t, *result);
    }
  printf ("The total = %f\n", total);
}
