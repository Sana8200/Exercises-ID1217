#include <stdio.h>
#include <pthread.h>

#define ROWS_A 2
#define COLS_A 3
#define ROWS_B 3
#define COLS_B 2

int A[ROWS_A][COLS_A] = {
    {1, 2, 3},
    {4, 5, 6}
};

int B[ROWS_B][COLS_B] = {
    {7, 8},
    {9, 10},
    {11, 12}
};

int C[ROWS_A][COLS_B];

void* compute_row(void* arg) {
    int i = *(int*)arg;    // row number

    for(int j = 0; j < COLS_B; j++){
        C[i][j] = 0;
        for(int k = 0; k < COLS_A; k++){
            C[i][j] += A[i][k] * B[k][j];
        }
    }
    // TODO: Calculate all elements in row i of C
    // C[i][j] = sum of A[i][k] * B[k][j] for all k
    return NULL;
}

int main() {
    pthread_t threads[ROWS_A];
    int row_indices[ROWS_A];
    
    // TODO: Create threads (one per row)
    for(int i = 0; i < ROWS_A ; i++){
        row_indices[i] = i;
        pthread_create(&threads[i], NULL, compute_row, &row_indices[i]);
    }
    
    // TODO: Wait for all threads
    for(int i = 0; i < ROWS_A ; i++){
        pthread_join(threads[i], NULL);
    }
    // Print result
    printf("Result C:\n");
    for (int i = 0; i < ROWS_A; i++) {
        for (int j = 0; j < COLS_B; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}