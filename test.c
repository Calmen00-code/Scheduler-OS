#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

void* foo(void *inA)
{
    int a;
    int *a_ptr;
    *((int*)inA) = 50;
    return inA;
}

int main()
{
    char str[100];
    double ave = 10.2;
    sprintf(str,"%s%f", "Average Turnaround time: ", ave);
    printf("%s", str);
    
    
    return 0;
}
