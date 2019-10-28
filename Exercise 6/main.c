#include "io.h"
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <native/task.h>
#include <native/timer.h>
#include <sys/mman.h>
#include <rtdk.h>



int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}



void read_and_return(void *input)
{
    long chan;
    chan = (long int)input;
    RTIME wait = 5000;
    rt_printf("Io channel %d \n", (int)chan);
    unsigned long duration = 100000000000;  // 10 second timeout
	unsigned long endTime = rt_timer_read() + duration;
    while (1)
    {
        if(!io_read(chan))
        {
            io_write(chan, 0);
            rt_timer_spin(wait);
            io_write(chan, 1);
        }
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
        rt_task_wait_period(NULL);
    }
}




void* redundant_task()
{
	set_cpu(1);
    while (1)
    {
        asm volatile("" ::: "memory");
    }
}


void task_A_I()
{
    RT_TASK A;
    RT_TASK B;
    RT_TASK C;
    unsigned long int a_chan = 1;
    unsigned long int b_chan = 2;
    unsigned long int c_chan = 3;
    rt_task_create(&A, "pollingA", 0, 50, T_CPU(1));
    rt_task_create(&B, "pollingB", 0, 50, T_CPU(1));
    rt_task_create(&C, "pollingC", 0, 50, T_CPU(1));
    rt_task_start(&A, read_and_return, (void *)a_chan);
    rt_task_start(&B, read_and_return, (void *)b_chan);
    rt_task_start(&C, read_and_return, (void *)c_chan);
    rt_task_join(&A);
    rt_task_join(&B);
    rt_task_join(&C);
}

void task_A_II()
{
    RT_TASK A;
    RT_TASK B;
    RT_TASK C;
    long int a_chan = 1;
    long int b_chan = 2;
    long int c_chan = 3;
    pthread_t I;
    pthread_t II;
    pthread_t III;
    pthread_t IV;
    pthread_t V;
    pthread_t VI;
    pthread_t VII;
    pthread_t VIII;
    pthread_t IX;
    pthread_t X;
    pthread_create(&I, NULL, redundant_task, NULL);
    pthread_create(&II, NULL, redundant_task, NULL);
    pthread_create(&III, NULL, redundant_task, NULL);
    pthread_create(&IV, NULL, redundant_task, NULL);
    pthread_create(&V, NULL, redundant_task, NULL);
    pthread_create(&VI, NULL, redundant_task, NULL);
    pthread_create(&VII, NULL, redundant_task, NULL);
    pthread_create(&VIII, NULL, redundant_task, NULL);
    pthread_create(&IX, NULL, redundant_task, NULL);
    pthread_create(&X, NULL, redundant_task, NULL);
    rt_task_create(&A, "pollingA", 0, 50, T_CPU(1));
    rt_task_create(&B, "pollingB", 0, 50, T_CPU(1));
    rt_task_create(&C, "pollingC", 0, 50, T_CPU(1));
    rt_task_start(&A, read_and_return, (void *)a_chan);
    rt_task_start(&B, read_and_return, (void *)b_chan);
    rt_task_start(&C, read_and_return, (void *)c_chan);
    rt_task_join(&A);
    rt_task_join(&B);
    rt_task_join(&C);
    pthread_join(I,NULL);
    pthread_join(II,NULL);
    pthread_join(III,NULL);
    pthread_join(IV,NULL);
    pthread_join(V,NULL);
    pthread_join(VI,NULL);
    pthread_join(VII,NULL);
    pthread_join(VIII,NULL);
    pthread_join(IX,NULL);
    pthread_join(X,NULL);
}


void task_B()
{
    RT_TASK A;
    RT_TASK B;
    RT_TASK C;
    pthread_t I;
    pthread_t II;
    pthread_t III;
    pthread_t IV;
    pthread_t V;
    pthread_t VI;
    pthread_t VII;
    pthread_t VIII;
    pthread_t IX;
    pthread_t X;
    RTIME period = 2670000;
    rt_task_set_periodic(&A, TM_NOW, period);
    rt_task_set_periodic(&B, TM_NOW, period);
    rt_task_set_periodic(&C, TM_NOW, period);
    pthread_create(&I, NULL, redundant_task, NULL);
    pthread_create(&II, NULL, redundant_task, NULL);
    pthread_create(&III, NULL, redundant_task, NULL);
    pthread_create(&IV, NULL, redundant_task, NULL);
    pthread_create(&V, NULL, redundant_task, NULL);
    pthread_create(&VI, NULL, redundant_task, NULL);
    pthread_create(&VII, NULL, redundant_task, NULL);
    pthread_create(&VIII, NULL, redundant_task, NULL);
    pthread_create(&IX, NULL, redundant_task, NULL);
    pthread_create(&X, NULL, redundant_task, NULL);
    long int a_chan = 1;
    long int b_chan = 2;
    long int c_chan = 3;
    rt_task_create(&A, "pollingA", 0, 50, T_CPU(1));
    rt_task_create(&B, "pollingB", 0, 50, T_CPU(1));
    rt_task_create(&C, "pollingC", 0, 50, T_CPU(1));
    rt_task_start(&A, read_and_return, (void *)a_chan);
    rt_task_start(&B, read_and_return, (void *)b_chan);
    rt_task_start(&C, read_and_return, (void *)c_chan);
    rt_task_join(&A);
    rt_task_join(&B);
    rt_task_join(&C);
    pthread_join(I,NULL);
    pthread_join(II,NULL);
    pthread_join(III,NULL);
    pthread_join(IV,NULL);
    pthread_join(V,NULL);
    pthread_join(VI,NULL);
    pthread_join(VII,NULL);
    pthread_join(VIII,NULL);
    pthread_join(IX,NULL);
    pthread_join(X,NULL);
}




int main()
{
    io_init();
    rt_print_auto_init(1);
    //rt_printf("1\n");
    mlockall(MCL_CURRENT | MCL_FUTURE);
    rt_printf("2\n");
    //task_A_I();
    //task_A_II();
    task_B();
    unsigned long duration = 10000000000;  // 10 second timeout
	unsigned long endTime = rt_timer_read() + duration;
    while (1)
    {
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
