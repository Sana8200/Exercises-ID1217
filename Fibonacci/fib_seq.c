
/* Recursively computes Fibonacci numbers   F(n) = F(n-1) + F(n-2)    F(0) = 0    F(1) = 1
   fib(n-1) and fib(n-2) are computed sequentially (one after the other). Inefficient algorithmically (exponential time),
*/
#include <stdlib.h>
#include <stdio.h>

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

int main(int argc, char *argv[]){
    long n = (argc > 1)? atoi(argv[1]) : 30;
    long result = fib(n);
    printf("fib(%ld) = %ld\n", n, result);
}