#include <stdio.h>
#include <pthread.h>

void *threadFunction(void *arg){
    printf("Hello from thread!\n");
    return NULL;
}

int main() {
    pthread_t thread;
    pthread_create(&thread, NULL, threadFunction, NULL);
    pthread_join(thread, NULL);
    return 0;
}
