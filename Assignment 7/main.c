#include "io.h"
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <native/task.h>
#include <native/timer.h>
#include <native/sem.h>
#include <sys/mman.h>
#include <rtdk.h>


int set_cpu(int cpu_number){
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);

	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpu);
}

void task_A()
{
	
}

unsigned long duration = 10000000000;  // 10 second timeout
unsigned long endTime = rt_timer_read() + duration;

int main()
{
    io_init();
    rt_print_auto_init(1);
    mlockall(MCL_CURRENT | MCL_FUTURE);
    
    return 0;
}
