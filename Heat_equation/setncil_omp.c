#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define T 10
#define N 100

double X1[N+2], X2[N+2];

int main(int argc, char *argv[]) {
    int t, i;
    int size = (argc > 1) ? atoi(argv[1]) : N;
    if (size > N) size = N;
    int numIterations = (argc > 2) ? atoi(argv[2]) : T;
    
    /* Initialize boundary conditions */
    X1[0] = X2[0] = 100.0;
    X1[size+1] = X2[size+1] = 100.0;
    
    /* 
     * #pragma omp parallel:
     *   - Creates a team of threads (default = number of cores)
     *   - private(t,i): each thread gets its own copy of t and i
     *   - num_threads(4): use 4 threads (optional, can omit)
     * 
     * All threads execute the code inside the parallel region.
     */
    #pragma omp parallel private(t,i) num_threads(4)
    {
        for (t = 0; t < numIterations; t++) {
            if ((t % 2) == 0) {
                /*
                 * #pragma omp :
                 *   - Distributes loop iterations among threads
                 *   - Implicit barrier at end: all threads wait
                 */
                #pragma omp for
                for (i = 1; i < size+1; i++)
                    X2[i] = (X1[i-1] + 2*X1[i] + X1[i+1]) / 4;
            } else {
                #pragma omp for
                for (i = 1; i < size+1; i++)
                    X1[i] = (X2[i-1] + 2*X2[i] + X2[i+1]) / 4;
            }
            /* Implicit barrier here - all threads sync before next timestep */
        }
    }
    
    /* Print result */
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