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
int main(int argc, char *argv[]){
	long n = (argc > 1)? atoi(argv[1]) : 30;
	long result;
	result = fib(n);
	printf("fib(%d) = %d\n", n, result);
}
