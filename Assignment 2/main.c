#include <stdlib.h>
#include <string.h>
#include "array.h"
#include <stdio.h>
#include <unistd.h>

void alloc_eight_gigs(void)
{
    long xy_size = 1000*1000*1000;
    long x_dim = 10000; //runs through with 1k-block and 10k-block, stops with 100-size block.
    long y_dim = xy_size/x_dim;

    long** matrix = malloc(y_dim * sizeof(long*));
    for (long y = 0; y < y_dim; ++y)
    {
        matrix[y] = malloc(x_dim*sizeof(long));
    }
    //difference between memory and swap is that the swap memory is a partition in the secondary memory.
}

void memset_eight_gigs(void) //segfault
{
    long xy_size = 1000*1000*1000;
    long x_dim = 10000;
    long y_dim = xy_size/x_dim;

    long** matrix = malloc(y_dim * sizeof(long*));
    for (long y = 0; y < y_dim; ++y)
    {
        matrix[y] = malloc(x_dim*sizeof(long));
        memset(matrix[y], 0, x_dim*sizeof(long));
    }
    //difference between memory and swap is that the swap memory is a partition in the secondary memory.
}



//Yields segfault when a large enough area in memory is overwritten,
//but does not yield segfault at smaller extensions of array
void array_ovf(void)
{
    Array this_array = array_new(9);
    for (int i = 0; i < 10; ++i)
    {
        ovf_array_insert_back(&this_array, i);
    }
    //new segfault when recompiled with -g -fsanitize=address
    //but this time with segfault regardless of size of the allocated memory
}

void safe_insert_back(void)
{
    Array this_array = array_new(1);
    int prev_array_size = 1;
    printf("starting array size: %d \n", prev_array_size);
    for (int i = 0; i < 300000; ++i)
    {
        if(prev_array_size < this_array.capacity)
        {
            printf("new array size: %ld \n", this_array.capacity);
            prev_array_size = this_array.capacity;
            sleep(1);
        }
        array_insert_back(&this_array, i);
    }
}


int main(void)
{
    //alloc_eight_gigs();
    //memset_eight_gigs();
    //array_ovf();
    //safe_insert_back();
    return 0;
}