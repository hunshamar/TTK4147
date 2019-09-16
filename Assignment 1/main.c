#include <stdio.h>
#include <unistd.h>
#include "copyable-code.h"
#include <sched.h>
#include <math.h>

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

void access_latency_rdtsc(void){
    for (int i = 0; i < 1000*1000*1000; ++i)
    {
        __rdtsc();
    }
    /*
        One billion measurements takes 8.63 seconds. Meaning that the access time is 8.63 ns
    */
}

void estimate_rdtsc_res(void)
{
    int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);



    long long int n = 10*1000*1000;
    unsigned int total = 0;
    for (int i = 0; i < n; ++i)
    {
        unsigned long long tick = __rdtsc();
        unsigned long long tock = __rdtsc();
        int ns = (int)floor((double)(tock - tick) / 2.66);

        if (ns >= 0 && ns < ns_max){  
            histogram[ns]++;
        }
    } 
    /*
        Avr ticks = 22.9 
    */

   for (int i = 0; i < ns_max; i++){
       printf("%d\n", histogram[i]);
   }


   return  (double)(total)/(double)n;
}


void estimate_clock_gettime_res(void)
{



    int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);

    int n = 10*1000*1000;
    struct timespec start, finish;
    for (int i = 0; i < n; ++i)
    {
        clock_gettime(CLOCK_REALTIME, &start);
        clock_gettime(CLOCK_REALTIME, &finish);
        long ns = finish.tv_nsec - start.tv_nsec;

        
        if (ns >= 0 && ns < ns_max){  
            histogram[ns]++;
        }
        
    }

    for (int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }


    
    //about 23.3 ns avg.
}

// void estimate_clock_gettime_res(void)
// {

    


//     int n = 1*1000*1000;
//     struct timespec start, finish, buf;
//     unsigned long int total_ns = 0;
//     clock_gettime(CLOCK_REALTIME, &start);
//     for (int i = 0; i < n; ++i)
//     {
//         clock_gettime(CLOCK_REALTIME, &buf);
//     }
//     clock_gettime(CLOCK_REALTIME, &finish);
//     long seconds = finish.tv_sec - start.tv_sec;
//     long ns = finish.tv_nsec - start.tv_nsec;
//     total_ns += (seconds * 1000*1000*1000);
//     total_ns += ns;
//     printf("clock_gettime avg. (ns) latency to measure: %fl \n ", (double)total_ns/(double)n);
//     //about 22.4 ns latency
//     return (double)total_ns/(double)n;
// }



void access_latency_times(){
    struct tms buff;
    for (int i = 0; i < 1000*1000; ++i)
    {
        times(&buff);
    }
    /*
        1 million measurements takes 0.468 seconds. Meaning that the access time is 468 ns
    */
}

int estimated_resolution_times()
{


    int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);


    struct tms buf;
    long int n = 10*1000*1000;
    unsigned int total = 0;
    for (int i = 0; i < n; ++i)
    {
        unsigned long long tick_ns = times(&buf);
        unsigned long long tock_ns = times(&buf);
        // printf("Tick: %ld, tock: %ld \n", tick_ns, tock_ns);
        // sleep(1);
        int ns = (tock_ns - tick_ns);

        if (ns >= 0 && ns < ns_max){  
            histogram[ns]++;
        }



    }
    for (int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }
    
   return (double)(total)/(double)n*10*1000*1000;
}

void context_switch()
{

    
    int ns_max = 500;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);


    long int n = 1000*1000;
    for (int i = 0; i < n; ++i){
        
        struct timespec start, finish;

        clock_gettime(CLOCK_REALTIME, &start);
        sched_yield();
        clock_gettime(CLOCK_REALTIME, &finish);
        long seconds = finish.tv_sec - start.tv_sec;
        long ns = finish.tv_nsec - start.tv_nsec;
         if (ns >= 0 && ns < ns_max){  
            histogram[ns]++;
        }
    }
    for (int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }

    
    //about 23.3 ns avg.
}


int main(void)
{
    //sleep(1);  // 1.001s real, reesten 0
      

    /*    
    struct timespec t;
    t.tv_sec = 1;

    busy_wait(t); // real 1.001, sys 1.000
    */


    //printf("Before wait\n");

    //busy_wait_times(1); // real 1.003, sys 1.000


    //printf("After wait\n");
    //estimate_rdtsc();
    //access_latency();
    //access_latency_times();
    //estimated_resolution_times();
    //estimate_clock_gettime_latency();



    //estimate_rdtsc_res();
    //estimate_clock_gettime_res();
    //estimated_resolution_times();


    context_switch();

    return 0;
}

