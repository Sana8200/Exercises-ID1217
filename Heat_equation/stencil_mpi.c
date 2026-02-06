#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define T 10
#define N 100

double X1[N+2], X2[N+2];

int main(int argc, char *argv[]) {
    int t, i;
    int p;          // Total number of processes
    int id;         // This process's rank (0, 1, 2, ... p-1)
    int size = N;
    int numIterations = T;
    MPI_Status status;
    
    /* Initialize MPI */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);   // Get total number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &id);  // Get this process's ID
    
    /* 
     * Calculate which portion of the array this process handles.
     * 
     * Example with p=4 processes and N=100:
     *   Process 0: indices 1-25   (mystart=1,  myend=26)
     *   Process 1: indices 26-50  (mystart=26, myend=51)
     *   Process 2: indices 51-75  (mystart=51, myend=76)
     *   Process 3: indices 76-100 (mystart=76, myend=101)
     */
    int mystart = id * size / p + 1;
    int myend = (id + 1) * size / p + 1;
    
    /* Initialize - each process sets its portion + boundaries */
    for (i = 0; i < N+2; i++) {
        X1[i] = X2[i] = 0.0;
    }
    
    /* Global boundaries (only process 0 and p-1 have real boundaries) */
    if (id == 0) {
        X1[0] = X2[0] = 100.0;  // Left boundary
    }
    if (id == p-1) {
        X1[size+1] = X2[size+1] = 100.0;  // Right boundary
    }
    
    /* Main computation loop */
    for (t = 0; t < numIterations; t++) {
        if ((t % 2) == 0) {
            /* Compute local portion */
            for (i = mystart; i < myend; i++)
                X2[i] = (X1[i-1] + 2*X1[i] + X1[i+1]) / 4;
            
            /* 
             * EXCHANGE BOUNDARY DATA WITH NEIGHBORS
             * 
             * Each process needs ghost cells from neighbors:
             *   - Need X2[mystart-1] from left neighbor
             *   - Need X2[myend] from right neighbor
             * 
             * Diagram for process 1 (middle process):
             * 
             *   Process 0          Process 1          Process 2
             *   [...|25|26]       [25|26|...|50|51]   [50|51|...]
             *         ↓ send              ↑ recv
             *         └──────────────────→┘
             *                    ↓ send         ↑ recv
             *                    └─────────────→┘
             */
            
            /* Send to right neighbor, receive from left neighbor */
            if (id != p-1) {  // Not the rightmost process
                MPI_Send(&X2[myend-1], 1, MPI_DOUBLE, id+1, 0, MPI_COMM_WORLD);
            }
            if (id != 0) {    // Not the leftmost process
                MPI_Recv(&X2[mystart-1], 1, MPI_DOUBLE, id-1, 0, MPI_COMM_WORLD, &status);
            }
            
            /* Send to left neighbor, receive from right neighbor */
            if (id != 0) {    // Not the leftmost process
                MPI_Send(&X2[mystart], 1, MPI_DOUBLE, id-1, 1, MPI_COMM_WORLD);
            }
            if (id != p-1) {  // Not the rightmost process
                MPI_Recv(&X2[myend], 1, MPI_DOUBLE, id+1, 1, MPI_COMM_WORLD, &status);
            }
        } 
        else {
            /* Same pattern for odd timesteps, but X1 ↔ X2 swapped */
            for (i = mystart; i < myend; i++)
                X1[i] = (X2[i-1] + 2*X2[i] + X2[i+1]) / 4;
            
            if (id != p-1) {
                MPI_Send(&X1[myend-1], 1, MPI_DOUBLE, id+1, 0, MPI_COMM_WORLD);
            }
            if (id != 0) {
                MPI_Recv(&X1[mystart-1], 1, MPI_DOUBLE, id-1, 0, MPI_COMM_WORLD, &status);
            }
            
            if (id != 0) {
                MPI_Send(&X1[mystart], 1, MPI_DOUBLE, id-1, 1, MPI_COMM_WORLD);
            }
            if (id != p-1) {
                MPI_Recv(&X1[myend], 1, MPI_DOUBLE, id+1, 1, MPI_COMM_WORLD, &status);
            }
        }
    }
    
    /* Gather and print results (simplified: only process 0 prints its portion) */
    if (id == 0) {
        printf("Process %d result: [ ", id);
        double *result = (numIterations % 2 == 0) ? X1 : X2;
        for (i = 0; i < myend; i++)
            printf("%.2f ", result[i]);
        printf("...]\n");
    }
    
    MPI_Finalize();
    return 0;
}