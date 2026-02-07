/*******************************************************************************
 * COMPUTING PI - PTHREADS VERSION
 *
 * Parallelisation strategy (SPMD - Single Program Multiple Data):
 *   - Each thread gets a unique ID (0, 1, 2, ..., nthreads-1)
 *   - Thread k processes iterations: k, k+nthreads, k+2*nthreads, ...
 *     (interleaved/cyclic distribution)
 *   - Each thread writes its partial sum to sum_array[id]
 *   - Main thread combines all partial sums after joining
 *
 * Why interleaved and not block distribution?
 *   Both work. Interleaved is simple and gives good load balance here
 *   since every rectangle has roughly the same computation cost.
 *
 * False sharing note:
 *   sum_array elements are adjacent in memory → different threads write
 *   to the same cache line → "false sharing" slows things down.
 *   For production code, pad each element to a full cache line (64 bytes).
 *   We show both versions below.
 *
 * Compile: gcc -O2 -o pi_pthreads pi_pthreads.c -lpthread -lm
 * Run:     ./pi_pthreads [num_threads] [num_steps]
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#define DEFAULT_NUM_STEPS   100000000L
#define DEFAULT_NUM_THREADS 4
#define CACHE_LINE_SIZE     64

/*---------------------------------------------------------------------------
 * Shared (global) state — all threads can read these
 *---------------------------------------------------------------------------*/
static long   num_steps;
static int    num_threads;
static double step;

/*
 * Padded partial sum to avoid false sharing.
 * Each thread's sum lives in its own cache line.
 */
typedef struct {
    double value;
    char   padding[CACHE_LINE_SIZE - sizeof(double)];
} padded_double_t;

static padded_double_t *partial_sums;   /* allocated at runtime */

/*---------------------------------------------------------------------------
 * Thread arguments
 *---------------------------------------------------------------------------*/
typedef struct {
    int thread_id;
} thread_arg_t;

/*---------------------------------------------------------------------------
 * Worker function — each thread runs this
 *---------------------------------------------------------------------------*/
void *compute_pi(void *arg)
{
    thread_arg_t *targ = (thread_arg_t *)arg;
    int id = targ->thread_id;

    double local_sum = 0.0;  /* accumulate locally first (register-friendly) */

    /*
     * Interleaved (cyclic) distribution:
     *   Thread 0: i = 0, nthreads, 2*nthreads, ...
     *   Thread 1: i = 1, nthreads+1, 2*nthreads+1, ...
     *   etc.
     */
    for (long i = id; i < num_steps; i += num_threads) {
        double x = (i + 0.5) * step;
        local_sum += 4.0 / (1.0 + x * x);
    }

    partial_sums[id].value = local_sum;  /* write once to shared array */
    return NULL;
}

/*---------------------------------------------------------------------------
 * Main
 *---------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    num_threads = DEFAULT_NUM_THREADS;
    num_steps   = DEFAULT_NUM_STEPS;

    if (argc > 1) num_threads = atoi(argv[1]);
    if (argc > 2) num_steps   = atol(argv[2]);

    step = 1.0 / (double)num_steps;

    /* Allocate padded partial sums array */
    partial_sums = calloc(num_threads, sizeof(padded_double_t));
    if (!partial_sums) {
        perror("calloc");
        return 1;
    }

    pthread_t   *threads = malloc(num_threads * sizeof(pthread_t));
    thread_arg_t *args   = malloc(num_threads * sizeof(thread_arg_t));

    struct timespec t_start, t_end;
    clock_gettime(CLOCK_MONOTONIC, &t_start);

    /* --- Create threads --- */
    for (int i = 0; i < num_threads; i++) {
        args[i].thread_id = i;
        if (pthread_create(&threads[i], NULL, compute_pi, &args[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    /* --- Wait for all threads to finish --- */
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    /* --- Combine partial sums (sequential reduction) --- */
    double total = 0.0;
    for (int i = 0; i < num_threads; i++) {
        total += partial_sums[i].value;
    }
    double pi = step * total;

    clock_gettime(CLOCK_MONOTONIC, &t_end);
    double elapsed = (t_end.tv_sec - t_start.tv_sec)
                   + (t_end.tv_nsec - t_start.tv_nsec) / 1e9;

    printf("Pthreads Pi Computation\n");
    printf("-----------------------\n");
    printf("Threads   : %d\n", num_threads);
    printf("Num steps : %ld\n", num_steps);
    printf("Pi        : %.15f\n", pi);
    printf("Error     : %.2e\n", fabs(pi - M_PI));
    printf("Time      : %.4f seconds\n", elapsed);

    free(partial_sums);
    free(threads);
    free(args);

    return 0;
}