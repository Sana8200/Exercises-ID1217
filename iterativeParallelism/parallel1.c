#include <stdio.h>
#include <pthread.h>

int arr[8] = {10, 20, 30, 40, 50, 60, 70, 80};

// Each thread runs this function
void *add_one(void* arg){
    int i = *(int*)arg;   // get index
    arr[i] = arr[i] + 1;   // add 1 to element
    return NULL;
}

int main(){
    int n = 8;
    pthread_t threads[8];
    int indices[8];

    // creating 8 threads, all running at the same time
    for(int i=0; i < n; i++){
        indices[i] = i;
        pthread_create(&threads[i], NULL, add_one, &indices[i]);
    }

    // wait for all threads to finish
    for(int i = 0; i < n; i++){
        pthread_join(threads[i], NULL);
    }

    // Print result
    printf("Result: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    return 0;
}