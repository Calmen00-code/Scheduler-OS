/**
 * Program: setup.c
 * Purpose: Setup and initialise value to a pointer array
 */

#include "setup.h"

/* Set all element in arr to integer val */
void set_arr( int *arr, int size, int val )
{
    int i;
    for ( i = 0; i < size; ++i )
        arr[i] = val;
}
