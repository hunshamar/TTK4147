#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

long global_iterator = 0;



sem_t mutex;

void* iterate_global_and_local(void * arg){
    
    long local_iterator = 0;
    int N = 50*1000*1000;
    for (int i = 0; i < N; i++){
        local_iterator++;

        sem_wait(&mutex);
        global_iterator++;
        sem_post(&mutex);
    }
    printf("Global iterator: %ld, local iterator %ld \n", global_iterator, local_iterator);
    return NULL; 
}



int main(){

    sem_init(&mutex, 0, 1);

    pthread_t firstThread, secondThread;

    pthread_create(&firstThread, NULL, iterate_global_and_local, NULL);
    pthread_create(&secondThread, NULL, iterate_global_and_local, NULL);
    
    pthread_join(firstThread, NULL);
    pthread_join(secondThread, NULL);
    
    /*
        Time:
        real	0m7.683s
        user	0m0.000s
        sys	    0m15.176s
    */


    
    return 0;
}