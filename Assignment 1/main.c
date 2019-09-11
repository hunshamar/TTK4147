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
    long long int n = 10*1000*1000;
    unsigned int total = 0;
    for (int i = 0; i < n; ++i)
    {
        unsigned long long tick = __rdtsc();
        unsigned long long tock = __rdtsc();
        total += (tock - tick);
    }
    printf("avg. ticks used for rdtsc: %fl \n", (double)(total)/(double)n);
    /*
        Avr ticks = 22.9 
    */
}


void estimate_clock_gettime(void)
{
    int n = 10*1000*1000;
    struct timespec start, finish;
    unsigned long int total_ns = 0;
    for (int i = 0; i < n; ++i)
    {
        clock_gettime(CLOCK_REALTIME, &start);
        clock_gettime(CLOCK_REALTIME, &finish);
        long seconds = finish.tv_sec - start.tv_sec;
        long ns = finish.tv_nsec - start.tv_nsec;
        total_ns += (seconds * 1000*1000*1000);
        total_ns += ns;
    }
    printf("clock_gettime avg. time(ns) between measurements: %fl", (double)(total_ns)/(double)n);
    //about 23.3 ns avg.
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
    //estimate_rdtsc();
    //access_latency();
    estimate_clock_gettime();
    printf("\nhello\n");
    
    return 0;
}

