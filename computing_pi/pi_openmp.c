/*******************************************************************************
 * COMPUTING PI - OPENMP VERSIONS
 *
 * Three progressively cleaner approaches:
 *
 *   Version 1: SPMD with manual partial sums (matches pthreads logic)
 *   Version 2: parallel for + critical section
 *   Version 3: parallel for + reduction   ← cleanest, recommended
 *
 * Compile: gcc -O2 -fopenmp -o pi_openmp pi_openmp.c -lm
 * Run:     ./pi_openmp [num_threads] [num_steps]
 *          OMP_NUM_THREADS=8 ./pi_openmp    (alternative)
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define DEFAULT_NUM_STEPS   100000000L
#define DEFAULT_NUM_THREADS 4
#define MAX_THREADS         256

/*===========================================================================
 * VERSION 1: SPMD — Manual partial sums (explicit thread management)
 *
 * This mirrors what we did with pthreads:
 *   - Each thread figures out its own ID and total thread count
 *   - Uses interleaved loop: for (i = id; i < N; i += nthreads)
 *   - Writes to a shared array; main thread reduces afterwards
 *
 * Pros: Full control, easy to understand mapping from pthreads
 * Cons: Verbose, manual reduction, potential false sharing
 *===========================================================================*/
double compute_pi_spmd(long num_steps, int req_threads)
{
    double step = 1.0 / (double)num_steps;
    double sum_array[MAX_THREADS];       /* partial sums (false sharing risk) */
    int    actual_threads;

    omp_set_num_threads(req_threads);

    #pragma omp parallel
    {
        int id       = omp_get_thread_num();    /* my thread ID: 0..nthreads-1 */
        int nthreads = omp_get_num_threads();   /* actual threads (may differ) */

        /* Only thread 0 records how many threads we actually got */
        if (id == 0) actual_threads = nthreads;

        double local_sum = 0.0;

        /* Interleaved distribution: thread k handles i = k, k+N, k+2N, ... */
        for (long i = id; i < num_steps; i += nthreads) {
            double x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }

        sum_array[id] = local_sum;
    }
    /* --- implicit barrier: all threads done --- */

    /* Sequential reduction */
    double total = 0.0;
    for (int i = 0; i < actual_threads; i++)
        total += sum_array[i];

    return step * total;
}

/*===========================================================================
 * VERSION 2: parallel for + critical
 *
 * OpenMP automatically splits the loop iterations across threads.
 * Each thread accumulates into a private local_sum, then adds to
 * the shared total inside a critical section (one thread at a time).
 *
 * Pros: Simpler than SPMD, no manual indexing
 * Cons: critical section is a serialisation point (small overhead here
 *       since it's only entered once per thread, not per iteration)
 *===========================================================================*/
double compute_pi_critical(long num_steps, int req_threads)
{
    double step  = 1.0 / (double)num_steps;
    double total = 0.0;

    omp_set_num_threads(req_threads);

    #pragma omp parallel
    {
        double local_sum = 0.0;

        /*
         * #pragma omp for — distributes loop iterations among threads.
         * Default schedule is usually static (N/nthreads contiguous chunks).
         */
        #pragma omp for
        for (long i = 0; i < num_steps; i++) {
            double x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }

        /*
         * critical: only one thread at a time executes this block.
         * Safe way to accumulate into shared variable.
         */
        #pragma omp critical
        {
            total += local_sum;
        }
    }

    return step * total;
}

/*===========================================================================
 * VERSION 3: parallel for + reduction   ★ RECOMMENDED ★
 *
 * The reduction clause tells OpenMP:
 *   1. Give each thread a private copy of 'sum' (initialised to 0)
 *   2. Each thread accumulates into its private copy
 *   3. After the loop, combine all copies using '+' automatically
 *
 * This is the cleanest and usually the fastest approach.
 * OpenMP handles all the partial-sum bookkeeping internally.
 *
 * Pros: Minimal code, no false sharing, no manual reduction
 * Cons: Less explicit (but that's a feature, not a bug)
 *===========================================================================*/
double compute_pi_reduction(long num_steps, int req_threads)
{
    double step = 1.0 / (double)num_steps;
    double sum  = 0.0;

    omp_set_num_threads(req_threads);

    #pragma omp parallel for reduction(+:sum)
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    return step * sum;
}

/*===========================================================================
 * Main — runs all three versions and compares
 *===========================================================================*/
int main(int argc, char *argv[])
{
    int  num_threads = DEFAULT_NUM_THREADS;
    long num_steps   = DEFAULT_NUM_STEPS;

    if (argc > 1) num_threads = atoi(argv[1]);
    if (argc > 2) num_steps   = atol(argv[2]);

    printf("OpenMP Pi Computation\n");
    printf("=====================\n");
    printf("Requested threads : %d\n", num_threads);
    printf("Num steps         : %ld\n\n", num_steps);

    double t0, t1, pi;

    /* --- Version 1: SPMD --- */
    t0 = omp_get_wtime();
    pi = compute_pi_spmd(num_steps, num_threads);
    t1 = omp_get_wtime();
    printf("V1 (SPMD manual)     : pi = %.15f  error = %.2e  time = %.4fs\n",
           pi, fabs(pi - M_PI), t1 - t0);

    /* --- Version 2: critical --- */
    t0 = omp_get_wtime();
    pi = compute_pi_critical(num_steps, num_threads);
    t1 = omp_get_wtime();
    printf("V2 (for + critical)  : pi = %.15f  error = %.2e  time = %.4fs\n",
           pi, fabs(pi - M_PI), t1 - t0);

    /* --- Version 3: reduction --- */
    t0 = omp_get_wtime();
    pi = compute_pi_reduction(num_steps, num_threads);
    t1 = omp_get_wtime();
    printf("V3 (for + reduction) : pi = %.15f  error = %.2e  time = %.4fs\n",
           pi, fabs(pi - M_PI), t1 - t0);

    return 0;
}