#include "io.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <rtdk.h>


// Set single CPU for pthread threads (NOT Xenomai threads!)

int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}


void* read_and_return(void *input)
{   
    struct responseTaskArgs a = *(struct responseTaskArgs*)input;
    printf("Io channel %d \n", a.test.channel);
    set_cpu(1);//task B
    while (1)
    {
        if(!io_read(a.test.channel))
        {
            io_write(a.test.channel, 0);
            usleep(50);
            io_write(a.test.channel, 1);
        }
    }
}


unsigned long duration = 10000000000;  // 10 second timeout
unsigned long endTime = rt_timer_read() + duration;

int main()
{    
    rt_print_auto_init(1);
    mlockall(MCL_CURRENT | MCL_FUTURE);
    
    while(1)
    {
        //do work

        if(rt_timer_read() > endTime)
        {
            rt_printf("Time expired\n");
            rt_task_delete(NULL);
        }
        if(rt_task_yield())
        {
            rt_printf("Task failed to yield\n");
            rt_task_delete(NULL);
        }
    }


    return 0;
}