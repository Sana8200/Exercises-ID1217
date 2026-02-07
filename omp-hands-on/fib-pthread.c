#ifndef _REENTRANT 
#define _REENTRANT 
#endif 
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
long fib(long n){
	long x, y;
	if (n < 2) return n;
	else {
		x = fib(n-1);
		y = fib(n-2);
	}
	return x + y;
}
typedef struct {
	long input;
	long output;
} thread_args;

void *thread_func(void *ptr){
	long i = ((thread_args *)ptr)->input;
	((thread_args *)ptr)->output = fib(i);
	return NULL;
}
int main(int argc, char *argv[]){
	pthread_t thread;
	thread_args args;
	long n = (argc > 1)? atoi(argv[1]) : 30;
	long result;
	if (n < 30) result = fib(n);
	else {
		args.input = n-1;
		pthread_create(&thread, NULL, thread_func, (void*)&args);
		result = fib(n-2);
		pthread_join(thread, NULL);
		result += args.output;
	}
	printf("fib(%d) = %d\n", n, result);
}

