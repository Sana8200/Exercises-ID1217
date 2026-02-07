#include <stdlib.h>
#include <stdio.h>
#define T 10
#define N 100

double X1[N+2], X2[N+2]; // initially zeros

int main(int argc, char *argv[]) {
 	int t, i;
	int size = (argc > 1)? atoi(argv[1]) : N;
	if (size > N) size = N;
	int numIterations = (argc > 2)? atoi(argv[2]) : T;
	// boundaries
	X1[0] = X2[0] = X1[size+1] = X2[size+1] = 100;
	for (t = 0; t < numIterations; t++) {
		if ((t % 2) == 0) {
			for (i = 1; i < size+1; i++)
      				X2[i] = (X1[i-1] + 2*X1[i] + X1[i+1]) / 4;
		} else {
			for (i = 1; i < size+1; i++)
				X1[i] = (X2[i-1] + 2*X2[i] + X2[i+1]) / 4;
		}
	}

  /* print the vector */
	printf("[ ");
	if (numIterations %2 == 0)
		for (i = 0; i < size+2; i++) 
			printf(" %f", X1[i]);
	else
		for (i = 0; i < size+2; i++)
			printf(" %f", X2[i]); 
	printf(" ]\n");
}
