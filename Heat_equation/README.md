A stencil computation updates each element of an array based on its neighbors. It's called a "stencil" because you can imagine placing a pattern (stencil) over each element to determine which neighbors contribute.

The 1D Heat Equation:
X[i]^(t+1) = (X[i-1]^t + 2*X[i]^t + X[i+1]^t) / 4

This simulates heat diffusion - each point's new temperature is the weighted average of itself and its neighbors.

Compilation and Running


Sequential:

gcc -o stencil_seq stencil_seq.c
./stencil_seq 100 10


OpenMP:

gcc -o stencil_omp stencil_omp.c -fopenmp
./stencil_omp 100 10


MPI:

mpicc -o stencil_mpi stencil_mpi.c
mpirun -np 4 ./stencil_mpi    # Run with 4 processes