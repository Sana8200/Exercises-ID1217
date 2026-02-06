#ifndef _REENTRANT
#define _REENTRANT
#endif

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

/* Recursive Fibonacci Funcaiton */
long fib(long n){
    long x,y;
    if(n < 2){
        return n;
    } else {
        x = fib(n - 1);
        y = fib(n - 2);
    }
    return x + y;
}

/* Structure to pass arguments to the thread and get results from the thread.
   pthread_create only allows passing one void pointer, so we bundle input and 
   output into a struct
*/
typedef struct{
    long input;      // The n value to compute fib(n)
    long output;     // Where the thread stores its first result
} thread_args;


/* The function that the new thread will execute
   must have:    void* function(void* args)
   parameters: void pointer to our thread_args struct
   Returns: NULL (we use the struct for the actual result)
*/
void *thread_func(void *ptr){
    // Cast the void pointer back to our struct type
    long i = ((thread_args *)ptr) -> input;

    // Compute fib and store result in the struct 
    ((thread_args *)ptr) -> output = fib(i);

    return NULL;
}


int main(int argc, char *argv[]){
    pthread_t thread;           // thread handler/identifire
    thread_args args;           // Struct for passing data to/from thread
    long n = (argc > 1)? atoi(argv[1]) : 30; 
    long result;

    // Only use parallelism for large n, thread creation has overhead, so for small n it's not worth it
    if(n < 30){
        result = fib(n);
    } else {
        /* Parallel Section */

        args.input = n-1;   // Set up the argument for the thread 

        /* pthread_create
           &thread        - where to store thread handler
           NULL           - defulat thread attributes
           thread_func    - function for thread to execute 
           &args          - arguments to pass to function   
        */
        pthread_create(&thread, NULL, thread_func, (void*)&args);

        result = fib(n-2);            // main thread computes fib(n-2) in parallel with new thread

        pthread_join(thread, NULL);   // wait for the spawned thread to finish, blocks untl thread_func returns
        
        result += args.output;         // Combine results,  args.output contains fib(n-1)
    }
    printf("fib(%ld) = %ld\n", n, result);
    return 0;
}