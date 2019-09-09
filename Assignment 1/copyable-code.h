#include <assert.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>
#include <x86intrin.h>




struct timespec timespec_normalized(time_t sec, long nsec);

struct timespec timespec_sub(struct timespec lhs, struct timespec rhs);

struct timespec timespec_add(struct timespec lhs, struct timespec rhs);

int timespec_cmp(struct timespec lhs, struct timespec rhs);

void busy_wait(struct timespec t);




/*
for(int i = 0; i < 10*1000*1000; i++){
    // read timer
}



int ns_max = 50;
int histogram[ns_max];
memset(histogram, 0, sizeof(int)*ns_max);

    for(int i = 0; i < 10*1000*1000; i++){
        
        // t1 = timer()
        // t2 = timer()
        
        int ns = // (t2 - t1) * ??
        
        if(ns >= 0 && ns < ns_max){
            histogram[ns]++;
        }
    }

    for(int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }





    long xy_size    = 1000*1000*1000;       // 8 GB (sizeof(long) = 8 bytes)
    long x_dim      = 100;
    long y_dim      = xy_size/x_dim;   
    
    long** matrix   = malloc(y_dim*sizeof(long*));

    for(long y = 0; y < y_dim; y++){
        matrix[y] = malloc(x_dim*sizeof(long));
    }

    printf("Allocation complete (press any key to continue...)\n");
    getchar();
*/