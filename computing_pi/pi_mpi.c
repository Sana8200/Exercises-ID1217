#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    int done = 0;
    int n, myid, numprocs, i;
    double mypi, pi, h, sum, x;
    double PI25DT = 3.141592653589793238462643;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    printf("I am process %d of %d\n", myid, numprocs);
    fflush(stdout);     

    while(!done){
        // Only process 0 reads input
        if(myid == 0){
            printf("Enter the number of intervals: (0 to quit) ");
            scanf("%d", &n);
        }

        // Broadcast n from process 0 to everyone all of the processes
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if(n == 0) break;

        h = 1.0 / (double) n;
        sum = 0.0;

        // Each process computes its portion (interleaved)
        // Process 0: i=1, 5, 9, ...
        // Process 1: i=2, 6, 10, ...  (if numprocs=4)

        for(i = myid + 1; i <= n; i += numprocs){
            x = h * ((double)i - 0.5);
            sum += 4.0 / (1.0 + x*x);
        }
        mypi = h * sum;     // Each process has a partial result

        // Sum (reduce) all of the partial results into pi at process 0
        MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        if(myid == 0){
            printf("pi is approximately %.16f, error is %.16f\n", pi, fabs(pi - PI25DT));
            fflush(stdout);  // flush to ensure output is printed before next prompt, especially when running with many processes
        }
        MPI_Finalize();
    }
}