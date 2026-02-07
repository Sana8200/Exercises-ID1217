Sequential — straightforward loop summing all rectangles. Baseline for comparison.

Pthreads — manual SPMD: each thread gets a unique ID, handles every Nth iteration 
           (interleaved/cyclic distribution), writes to a cache-line-padded partial sum array to avoid false sharing. Main thread joins and reduces.
           
OpenMP — three progressively cleaner versions:

V1 (SPMD): Same manual logic as pthreads, but thread creation/joining handled by #pragma omp parallel
V2 (for + critical): #pragma omp for auto-distributes iterations; each thread accumulates locally then adds via #pragma omp critical
V3 (for + reduction): #pragma omp parallel for reduction(+:sum) — one line does everything. This is the idiomatic OpenMP way.