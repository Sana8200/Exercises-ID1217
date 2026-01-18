#include <stdio.h>

int main(){
    int arr[5] = {5, 10, 15, 20, 25};
    int n = 5;

    // Sequential : multiply each by 2 
    for (int i=0; i < n; i++){
        arr[i] = arr[i] * 2;
    }

    //print result 
    for (int i=0; i < n; i++){
        printf("%d\n", arr[i]);
    }
    return 0;
}