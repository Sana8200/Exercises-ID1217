#include <stdio.h>
#include <pthread.h>


int main(){
    int arr[8] = {10, 20, 30, 40, 50, 60, 70, 80};
    int n = 8;
    
    // Sequential, one element at a time 
    for(int i=0; i < n; i++){
        arr[i] = arr[i] + 1; 
    }

    
    // Print the updated array
    for(int i=0; i < n; i++){
        printf("%d\n", arr[i]);
    }
    return 0;
}