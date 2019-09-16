#include <stdlib.h>
#include <string.h>

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
        memset(matrix[y], 0, x_dim*sizeof(long));
    }
    //difference between memory and swap is that the swap memory is a partition in the secondary memory.
}

int main(void)
{
    //alloc_eight_gigs();
    //memset_eight_gigs();

    return 0;
}

