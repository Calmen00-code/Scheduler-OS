/**
 * Program: result.c
 * Purpose: Computing the result of the gantt chart, ave turnaround and wait time
 * Author : Calmen Chia
 */

#include <stdio.h>
#include <string.h>
#include "Task.h"

/** 
 * Operation to print the gantt chart of the process 
 */
void gantt_chart( WriteTask *wrt_task, int wrt_size, int start_time )
{ 
    int i, j, idx;

    /* Printing the top line of the gantt chart */
    for ( i = 0; i < wrt_size; ++i ) {
        /* The size of wrt_task is set bigger than actual in advance
           Therefore, a status to check if it was WRITTEN is needed */
        if ( wrt_task[i].status == WRITTEN ) {
            /* Printing bottom line based on distance between 
               previous and current burst time */
            if ( i > 0 ) 
                idx = i-1;
            else 
                idx = 0;
            for ( j = wrt_task[idx].turnaround; j < wrt_task[i].turnaround; ++j )
                printf("---");
        }
    }
    printf("\n");

    if ( start_time > 0 )
        printf("  ");
    /* Printing the Process Label */
    for ( i = 0; i < wrt_size; ++i ) {
        /* The size of wrt_task is set bigger than actual in advance
           Therefore, a status to check if it was WRITTEN is needed */
        if ( wrt_task[i].status == WRITTEN ) {
            printf("|%s", wrt_task[i].label);

            /* Printing space based on distance between 
               previous and current burst time */
            if ( i > 0 ) 
                idx = i-1;
            else 
                idx = 0;
            for ( j = wrt_task[idx].turnaround; j < wrt_task[i].turnaround; ++j ) 
                printf(" ");
        }
    }
    printf("|");    /* Closing Bracket for last Process */
    printf("\n");

    /* Printing the bottom line of the gantt chart */
    for ( i = 0; i < wrt_size; ++i ) {
        /* The size of wrt_task is set bigger than actual in advance
           Therefore, a status to check if it was WRITTEN is needed */
        if ( wrt_task[i].status == WRITTEN ) {
            /* Printing bottom line based on distance between 
               previous and current burst time */
            if ( i > 0 ) 
                idx = i-1;
            else 
                idx = 0;
            for ( j = wrt_task[idx].turnaround; j < wrt_task[i].turnaround; ++j )
                printf("---");
        }
    }
    printf("\n");

    if ( start_time > 0 )
        printf("0 ");
    printf("%d", start_time);
    for ( i = 0; i < wrt_size; ++i ) {
        /* The size of wrt_task is set bigger than actual in advance
           Therefore, a status to check if it was WRITTEN is needed */
        if ( wrt_task[i].status == WRITTEN ) {
            /* Printing bottom line based on distance between 
               previous and current burst time */
            if ( i > 0 ) 
                idx = i-1;
            else 
                idx = 0;
            for ( j = wrt_task[idx].turnaround; j < wrt_task[i].turnaround; ++j )
                printf(" ");
            printf(" %d ", wrt_task[i].turnaround);
        }
    }
    printf("\n");
}

/**
 * Return the average waiting time of the processes 
 */
double ave_wait_time( WriteTask *wrt_task, int wrt_size )
{
    int i, actual_size, wait_time;
    double ave, sum;

    /**                                                     **
     *         CALCULATE SUM OF WAITING TIME                 *
     *  The previous process's turnaround time is the        *
     *  time when current process get execute in CPU         *
     *                                                       *
     *  waiting time = previous process turnaround time -    *
     *                 current arrival time                  *
     *                                                       *
     *  i = 1 as First Process always has 0 waiting time     *
     **                                                     **/
    sum = 0.0;
    for ( i = 1; i < wrt_size - 1; ++i ) {
        /* Waiting Time = Previous Turnaround Time - Current Arrival Time */
        /* Check if idle time of CPU */
        /* If current label does not contains 'P', 
           it there are no process at this time */
        if ( wrt_task[i].status == WRITTEN && 
             strchr(wrt_task[i].label, 'P') != NULL ) {
            wait_time = wrt_task[i-1].turnaround - wrt_task[i].arrival;
            sum += (double)wait_time;
        }
    }

    actual_size = 0;
    for ( i = 0; i < wrt_size; ++i ) {
        /* Check if idle time of CPU */
        /* If current label does not contains 'P', 
           it there are no process at this time */
        if ( wrt_task[i].status == WRITTEN && 
             strchr(wrt_task[i].label, 'P') != NULL ) {
            ++actual_size;
        }
    }
    ave = sum / (double)actual_size;
    return ave;
}

/**
 * Return the average turn around time of the processes 
 */
double ave_turnaround_time( WriteTask *wrt_task, int wrt_size )
{
    int i, actual_size;
    double ave, sum;

    sum = 0.0;
    /* Turnaround Time = Finished Time - Arrival Time */
    for ( i = 0; i < wrt_size; ++i ) {
        /* Check if idle time of CPU */
        /* If current label does not contains 'P', 
           it there are no process at this time */
        if ( wrt_task[i].status == WRITTEN && 
             strchr(wrt_task[i].label, 'P') != NULL ) {
            sum += wrt_task[i].turnaround - wrt_task[i].arrival;
        }
    }

    actual_size = 0;
    for ( i = 0; i < wrt_size; ++i ) {
        /* Check if idle time of CPU */
        /* If current label does not contains 'P', 
           it there are no process at this time */
        if ( wrt_task[i].status == WRITTEN && 
             strchr(wrt_task[i].label, 'P') != NULL ) {
            ++actual_size;
        }
    }
    ave = sum / (double)actual_size;
    return ave;
}
