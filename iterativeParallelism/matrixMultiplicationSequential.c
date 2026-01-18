#include <stdio.h>

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

int main() {
    // Calculate C = A × B
    for (int i = 0; i < ROWS_A; i++) {
        for (int j = 0; j < COLS_B; j++) {
            C[i][j] = 0;
            for (int k = 0; k < COLS_A; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    
    // Print result
    printf("Result C:\n");
    for (int i = 0; i < ROWS_A; i++) {
        for (int j = 0; j < COLS_B; j++) {
            printf("%d  ", C[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}