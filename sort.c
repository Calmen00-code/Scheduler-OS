/**
 * Program: sort.c
 * Purpose: Sorts the tasks based on their Arrival Time
 * Author : Calmen Chia
 */

#include <stdio.h>
#include "sort.h"

/**
 * Sorts the tasks array according to the arrival times 
 * in ascending order
 */
void bubble_sort_pp( TaskPP *tasks, int size )
{
    TaskPP tmp;
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

/**
 * Sorts the tasks array according to the arrival times 
 * in ascending order
 */
void bubble_sort_srtf( TaskSRTF *tasks, int size )
{
    TaskSRTF tmp;
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
