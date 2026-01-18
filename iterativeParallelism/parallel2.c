#include <stdio.h>
#include <pthread.h>

int arr[5] = {5, 10, 15, 20, 25};

void* multiplay(void* args){
    int index = *(int*)args;
    arr[index] = arr[index] * 2;
    return NULL;
}

int main(){
    int n = 5;
    pthread_t threads[n];
    int indecies[n];

    // Create threads
    for(int i=0; i < n ; i++){
        indecies[i] = i;
        pthread_create(&threads[i], NULL, multiplay, &indecies[i]);
    }

    // Wait for threads to finish 
    for(int i=0; i<n ; i++){
        pthread_join(threads[i], NULL);
    }

    // Print result
    for(int i=0; i<n ; i++){
        printf("%d\n", arr[i]);
    }

    return 0;
}