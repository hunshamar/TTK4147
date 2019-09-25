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
    }
    printf("Global iterator: %ld, local iterator %ld \n", global_iterator, local_iterator);
    return NULL; 
}



int main(){
    pthread_t firstThread;
    pthread_t secondThread;

    pthread_create(&firstThread, NULL, iterate_global_and_local, NULL);
    pthread_create(&secondThread, NULL, iterate_global_and_local, NULL);
    
    pthread_join(firstThread, NULL);
    pthread_join(secondThread, NULL);
    
    /*
        Time:
        real: 0m0.191s
        user: 0m0.000s
        sys:  0m0.368s

    */
        
    
    return 0;
}