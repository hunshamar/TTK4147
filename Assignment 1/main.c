#include <stdio.h>
#include <unistd.h>
#include "copyable-code.h"


void busy_wait_times(int s)
{
    struct tms buf;
    time_t timestamp1 = times(&buf);

    time_t timestamp2 = timestamp1;
    while(timestamp2 <= timestamp1 + 100*s)
    {
        timestamp2 = times(&buf);
    }
}

void access_latency(void){
    for (int i = 0; i < 1000*1000*1000; ++i)
    {
        __rdtsc();
    }
    /*
        One billion measurements takes 8.63 seconds. Meaning that the access time is 8.63 ns
    */
}

void estimate_rdtsc(void)
{
    long int n = 10*1000*1000;
    unsigned int total = 0;
    for (int i = 0; i < n; ++i)
    {
        unsigned long long tick = __rdtsc();
        unsigned long long tock = __rdtsc();
        total += (tock - tick);
    }
    printf("avg. ticks used for rdtsc: %fl", (double)(total)/(double)n);
    /*
        Avr ticks = 22.9 
    */
}

void estimate_clock_gettime(void)
{
    unsigned int total
    struct timespec tp;
    clockid_t clk_id = CLOCK_MONOTONIC;
    long int n = 10*1000*1000;
    for (int i = 0; i < n; ++i)
    {
        int tick = clock_gettime(clk_id, &tp);
        int tock = clock_gettime(clk_id, &tp);
        total+= (tick - tock)
    }
    printf("avg. time between measurements: %fl", double)
}

int main(void)
{
    //sleep(1);
    

    /*
    struct timespec t;
    t.tv_sec = 1;

    busy_wait(t);
    */


    //printf("Before wait\n");

    //busy_wait_times(1);


    //printf("After wait\n");
    estimate_rdtsc();
    //access_latency();

    
    return 0;
}

