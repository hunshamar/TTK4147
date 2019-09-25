#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

long global_iterator = 0;


void* iterate_global_and_local(void * arg){
    
    long local_iterator = 0;
    int N = 50*1000*1000;
    for (int i = 0; i < N; i++){
        local_iterator++;
        global_iterator++;
        if (N % 5*1000*1000 == 0){
            //printf("N =  %d \n", N);
        }
    }
    printf("Global iterator: %ld, local iterator %ld \n", local_iterator, global_iterator);
    return NULL; 
}



int main(){
    pthread_t firstThread;
    pthread_t secondThread;

    pthread_create(&firstThread, NULL, iterate_global_and_local, NULL);
    pthread_create(&secondThread, NULL, iterate_global_and_local, NULL);
    
    
    
    
    return 0;
}