/*******************************************************************************
 * COMPUTING PI - SEQUENTIAL VERSION
 *
 * Math:  π = ∫₀¹ 4/(1+x²) dx
 *
 * We approximate using the midpoint rule:
 *   - Divide [0,1] into N rectangles, each of width step = 1/N
 *   - For rectangle i, midpoint xᵢ = (i + 0.5) * step
 *   - Sum all rectangle heights: Σ 4/(1 + xᵢ²)
 *   - Multiply by step to get area ≈ π
 *
 * Compile: gcc -O2 -o pi_seq pi_sequential.c -lm
 * Run:     ./pi_seq [num_steps]
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define DEFAULT_NUM_STEPS 100000000L  /* 10^8 */

int main(int argc, char *argv[])
{
    long num_steps = DEFAULT_NUM_STEPS;
    if (argc > 1)
        num_steps = atol(argv[1]);

    double step = 1.0 / (double)num_steps;
    double sum  = 0.0;

    struct timespec t_start, t_end;
    clock_gettime(CLOCK_MONOTONIC, &t_start);

    /*
     * Core loop: accumulate f(xᵢ) for each midpoint.
     * We factor out 'step' and multiply once at the end.
     */
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;   /* midpoint of rectangle i */
        sum += 4.0 / (1.0 + x * x);    /* f(x) = 4/(1+x²)        */
    }

    double pi = step * sum;

    clock_gettime(CLOCK_MONOTONIC, &t_end);
    double elapsed = (t_end.tv_sec - t_start.tv_sec)
                   + (t_end.tv_nsec - t_start.tv_nsec) / 1e9;

    printf("Sequential Pi Computation\n");
    printf("-------------------------\n");
    printf("Num steps : %ld\n", num_steps);
    printf("Pi        : %.15f\n", pi);
    printf("Error     : %.2e\n", fabs(pi - M_PI));
    printf("Time      : %.4f seconds\n", elapsed);

    return 0;
}