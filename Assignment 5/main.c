#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
#include <unistd.h>
#include <stdint.h>

#include "io.h"


struct responseTaskArgs{
    struct{
        uint32_t channel;
    }test;
};



void* read_and_return(void *input)
{   
    struct responseTaskArgs a = *(struct responseTaskArgs*)input;
    printf("Io channel %d \n", a.test.channel);
    while (1)
    {
        if(io_read(a.test.channel))
        {
            io_write(a.test.channel, 0);
            usleep(10);
            io_write(a.test.channel, 1);
        }
    }
}


int main()
{
    io_init();
    pthread_t A;
    pthread_t B;
    pthread_t C;



    pthread_create(&A, NULL, read_and_return, (&((struct responseTaskArgs){{1}})));
    pthread_create(&B, NULL, read_and_return, (&((struct responseTaskArgs){{2}})));
    pthread_create(&C, NULL, read_and_return, (&((struct responseTaskArgs){{3}})));


    pthread_join(A,NULL);
    pthread_join(B,NULL);
    pthread_join(C,NULL);


    return 0;
}