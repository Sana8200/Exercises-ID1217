#include <stdlib.h>
#include <stdio.h>

#define T 10      // Number of time iterations
#define N 100     // Array size

double X1[N+2];         // Two array for double-buffering
double X2[N+2];         // size N+2 to include boundary elements

int main(int argc, char *argv[]){
    int t,i;

    /* command line args */
    int size = (argc > 1) ? atoi(argv[1]) : N;
    if(size > N) size = N;
    int numIterations = (argc > 2) ? atoi(argv[2]) : T;

    /* Initialize arrays to zero(global arrays are zero by default)
       set boundry conditions: both ends fixed at 100 degrees
    */
    X1[0] = X2[0] = 100.0;
    X1[size + 1] = X2[size + 1] = 100.0;

    // Main computation loop 
    for(t = 0; t < numIterations; t++){
        if((t%2) == 0){
            // Even iterations: read from X1, write to X2
            for (i = 1; i < size+1; i++)
                X2[i] = (X1[i-1] + 2*X1[i] + X1[i+1]) / 4;
        } else {
            // Odd iterations: read from X2, write to X1
            for (i = 1; i < size+1; i++)
                X1[i] = (X2[i-1] + 2*X2[i] + X2[i+1]) / 4;
        }
    }

    /* Print final result */
    printf("[ ");
    if (numIterations % 2 == 0)
        for (i = 0; i < size+2; i++) 
            printf("%.2f ", X1[i]);
    else
        for (i = 0; i < size+2; i++)
            printf("%.2f ", X2[i]);
    printf("]\n");
    
    return 0;
}