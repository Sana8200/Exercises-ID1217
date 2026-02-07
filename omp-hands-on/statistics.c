#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

int is_master() {
	return omp_get_thread_num() == 0;
}

int main() {
	printf("Nested parallel regions are enabled, %d\n", omp_get_nested());
	//omp_set_nested(0);
	printf("Nested parallel regions are enabled, %d\n\n", omp_get_nested());

	printf("Dynamic threads are enabled, %d\n", omp_get_dynamic());
	//omp_set_dynamic(0);
	printf("Dynamic threads are enabled, %d\n\n", omp_get_dynamic());

	printf("Thread %d in sequential section\n", omp_get_thread_num());
	printf("%d threads exist in sequential section\n\n", omp_get_num_threads());

#pragma omp parallel
	{
		int thread_id= omp_get_thread_num();
		printf("Thread %d in parallel section\n", thread_id);

		if (is_master()) {
			printf("%d threads exist in parallel section\n", omp_get_num_threads());
		}

#pragma omp single
		{
			printf("Thread %d in single section\n", thread_id);
		}
	}
	printf("\n");

	int iterations = 20;
	int** my_iterations = (int**) malloc(sizeof(int) * omp_get_max_threads());
	int* no_of_iterations = (int*) malloc(sizeof(int) * omp_get_max_threads());
	for (int i=0; i<omp_get_max_threads(); i++) {
		my_iterations[i] = (int*) malloc(sizeof(int) * iterations);
		no_of_iterations[i] = 0;
	}

	printf("iteration no:");
	#pragma omp parallel for
	//#pragma omp parallel for schedule(static) ordered
//#pragma omp parallel for schedule(static, 1)
//	#pragma omp parallel for schedule(dynamic)
	for (int i=0; i<iterations; i++) {
		printf(" %d", i);
		my_iterations[omp_get_thread_num()][no_of_iterations[omp_get_thread_num()]++] = i;
	}
	printf("\n");

	for (int i=0; i<omp_get_max_threads(); i++) {
		printf("Thread %d executed iteration:", i);
		for (int j=0; j<no_of_iterations[i]; j++) {
			printf(" %d", my_iterations[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
