#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

long fib(long n){
    long x,y;
    if(n < 2){
        reurn n;
    } else{
        #pragma omp task shared(x)
        x = fib(n-1);

        #pragma omp task shared(y)
        y = fib(n-2);

        #pragma omp taskwait
    }
    return x+y;
}


int main(int argc, char *argv[]){
    long n = (arg > 1)? atoi(arg[1]): 30;
    long result;

    #pragma omp parallel{
        #pragma omp single {
            result = fib(n);
        }
    }
    printf("fib(%ld) = %ld\n", n, result);
}