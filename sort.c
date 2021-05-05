/**
 * Program: sort.c
 * Purpose: Sorts the tasks based on their Arrival Time
 * Author : Calmen Chia
 */

#include <stdio.h>
#include "Task.h"   /* Allow the use of Task struct */

/**
 * Sorts the tasks array according to the arrival times 
 * in ascending order
 */
void bubble_sort( Task *tasks, int size )
{
    Task tmp;
    int i, j;
    for ( i = 0; i < size; ++i ) {
        for ( j = 0; j < size; ++j ) {
            if ( tasks[j].arrival > tasks[i].arrival ) {
                tmp = tasks[j];
                tasks[j] = tasks[i];
                tasks[i] = tmp;
            }
        }
    }
}
