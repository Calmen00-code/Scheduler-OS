/**
 * Program: schedule.c
 * Purpose: Scheduling functions for Priority Preemptive
 * Author : Calmen Chia
 */

#include <stdio.h>  
#include <string.h> 
#include <stdlib.h>
#include "header.h"
#include "srtf.h"

/**
 * Main function to run the scheduling PP Algorithm
 */
WriteTask* process_srtf( TaskSRTF *tasks, int task_size, int wrt_size )
{
    int flag_time, total_burst_time, i, ii, start_time;
    int curr_arrival, total_idle_time, idle_time;
    double ave_turnaround, ave_wait;
    char space[STR];
    TaskSRTF *running_task;
    WriteTask *wrt_task;

    /* Allocation of wrt_task */
    wrt_task = malloc(sizeof(WriteTask) * wrt_size);
    for ( ii = 0; ii < wrt_size; ++ii ) {
        wrt_task[ii].turnaround = 0;
        strcpy(wrt_task[ii].label, "");
        wrt_task[ii].status = UNWRITTEN;
    }

    total_burst_time = sum_burst(tasks, task_size);

    /* Check if the first tasks did not start from 0 (Eg: 1,2,3...n) */
    /* If first task did not start from 0, increment the flag_time */
    flag_time = 0;
    if ( tasks[0].arrival > 0 ) {
        flag_time = tasks[0].arrival;
        /* The start time for wrt_task will be arrival time for first task */
        start_time = flag_time;  
    } else  /* Otherwise, the task is start at time 0 */
        start_time = 0;

    /* Selecting process to be allocated in the CPU from time to time */
    i = 0; total_idle_time = 0;
    while ( flag_time < total_burst_time ) {
        if ( hasProcess( tasks, task_size, flag_time ) == TRUE ) {
            running_task = priority(flag_time, tasks, task_size); 
            CPU(tasks, task_size, running_task, &wrt_task[i], &flag_time);
        } else {
            idle_time = 0;
            curr_arrival = flag_time;
            /* Append spaces everytime when there was no process found */
            while ( hasProcess( tasks, task_size, flag_time ) == FALSE ) {
                strcat(space, " ");
                ++flag_time;
            }
 
            idle_time = flag_time - curr_arrival;
            total_idle_time += idle_time;

            /* Writing to the wrt_task */
            strcpy(wrt_task[i].label, space);
            wrt_task[i].arrival = curr_arrival;
            wrt_task[i].turnaround = flag_time;
            wrt_task[i].status = WRITTEN; 
            total_burst_time += total_idle_time;
        }
        ++i;
    }

    return wrt_task;
}

/**
 * Return TRUE if there are processes arrived from 0 to flag_time
 */
int hasProcess( TaskSRTF *tasks, int task_size, int flag_time )
{
    int i, exist;

    /* Iterate until a process arrived at flag_time is found */
    i = 0; exist = FALSE;
    while ( i < task_size && exist == FALSE ) {
        /* All process before flag_time will be accepted */
        if ( tasks[i].arrival <= flag_time && tasks[i].burst != 0 )
            exist = TRUE;
        ++i;
    }
    return exist;
}

/**
 * Import : flag_time -> time from 0 up until the latest/current runtime
 *          tasks     -> decide which specific tasks[i] to be returned
 *
 * Purpoose: Return the highest priority task for all the tasks starting from 0 to flag_time
 */
TaskSRTF* priority( int flag_time, TaskSRTF *tasks, int task_size )
{
    int i, j, k, pr_burst, idx;
    int pr_idx;         /* Highest priority index for undone task */
    int *undone_idx;    /* Indexes for all undone tasks */
    TaskSRTF *ret_task;

    /* Setup the undone_idx */
    undone_idx = calloc(sizeof(int), task_size);
    set_arr(undone_idx, task_size, EMPTY_IDX);

    /**            
     * Stops when current arrival time for tasks[i] is
     * greater than current latest time (flag_time)  
     *
     * Store the index of all undone tasks
     */
    i = 0; j = 0;
    while ( i < task_size && tasks[i].arrival <= flag_time ) {
        /* Check if this task had finished execution */
        if ( tasks[i].burst != 0 ) {
            undone_idx[j] = i;
            ++j;
        }
        ++i;
    }

    /* Get the first undone priority */
    pr_idx = undone_idx[0];
    pr_burst = tasks[pr_idx].burst;

    /* Find the highest priority among all undone task entries */
    for ( k = 1; k < task_size; ++k ) {
        if ( undone_idx[k] != EMPTY_IDX ) {
            /* Compare current priority with the latest highest priority */
            /* Lower Value indicates higher priority */
            idx = undone_idx[k];
            if ( tasks[idx].burst < pr_burst ) {
                pr_idx = idx;                     /* Update the latest index */ 
                pr_burst = tasks[idx].burst;      /* Update the latest burst */
            }
        }
    }
    ret_task = &tasks[pr_idx];
    free(undone_idx); undone_idx = NULL;
    return ret_task;
}

/**
 * Import: tasks        -> For the use of getting next preempt information
 *         running_task -> Decrement on the burst time of current running task
 *         wrt_task     -> Write the process with its remaining burst time
 *         *flag_time   -> Update the flag_time in process function 
 *                      -> Store its final value into wrt_task[i]
 *
 * Purpose: Performs burst_time decrement on the running_task
 */
void CPU( TaskSRTF *tasks, int task_size, TaskSRTF *running_task, 
          WriteTask *wrt_task, int *flag_time )
{
    int stop = FALSE;

    /**
     * Stops when higher priority task had preempted OR
     * when current task done execution
     */
    while ( stop == FALSE && running_task->burst > 0 ) {
        running_task->burst--;
        /* Update flag_time in process function (its caller) */
        *flag_time = *flag_time + 1; 

        /* Check if the newly arrived process will preempt current process */
        if ( isPreempt(tasks, task_size, 
                       *flag_time, running_task ) == TRUE ) {
            stop = TRUE;
        }
    }

    /* Writing to the wrt_task */
    strcpy(wrt_task->label, running_task->label);
    wrt_task->arrival = running_task->arrival;
    wrt_task->turnaround = *flag_time;
    wrt_task->status = WRITTEN; 
}

/**
 * Compare the priority of the newly arrived process and 
 * return TRUE if newly arrived process had higher priority
 */
int isPreempt( TaskSRTF *tasks, int task_size, int curr_time, TaskSRTF *running_task )
{
    int i, j, ii;
    int *undone_idx, idx;
    int preempt = FALSE;

    undone_idx = calloc(sizeof(int), task_size); 
    set_arr(undone_idx, task_size, EMPTY_IDX);

    j = 0;
    for ( i = 0; i < task_size; ++i ) {
        /* Store all index for arrived AND undone task */
        if ( tasks[i].arrival <= curr_time && 
             tasks[i].burst != 0 ) {
            undone_idx[j] = i;
            ++j;
        }
    }
    
    ii = 0;
    while ( ii < task_size && preempt == FALSE ) {
        idx = undone_idx[ii];  /* Get the idx for undone task */

        /* Compute only all available index */
        if ( undone_idx[ii] != EMPTY_IDX ) {
            /* Check if preemption is possible */
            if ( tasks[idx].burst < running_task->burst )
                preempt = TRUE;
        }
        ++ii;
    }
    free(undone_idx); undone_idx = NULL;
    return preempt;
}

/**
 * Inform the next preempt time and index from current running task 
 */
int next_preempt( TaskSRTF *tasks, int task_size, 
                  TaskSRTF *running_task, int *preempt_idx )
{
    int i, j;
    int time;

    /**
     * Find the entries that stores running_task in tasks
     *
     * Using the comparison of address in each entries of task[i]
     * with the address of running_task 
     */
    i = 0;
    while ( i < task_size && strcmp(tasks[i].label, running_task->label) != 0 )
        ++i;
 
    /* ASSERTION: task[i].arrival > running_task->arrival */
    /* Stops when next arrival time was found */
    j = i;
    while ( j < task_size && tasks[j].arrival == running_task->arrival )
        ++j;

    /* Return Next Preempt Index and Next Preempt Time */
    if ( j < task_size ) {
        *preempt_idx = j;
        time = tasks[j].arrival;
    }
    return time;
}

/** 
 * Operation to print the gantt chart of the process 
 */
void gantt_chart( WriteTask *wrt_task, int wrt_size, 
                  int start_time, char *output )
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
                sprintf(output, "%s", "---");
        }
    }
    sprintf(output, "%s", "\n");

    if ( start_time > 0 )
        sprintf(output, "%s", "  ");
    /* Printing the Process Label */
    for ( i = 0; i < wrt_size; ++i ) {
        /* The size of wrt_task is set bigger than actual in advance
           Therefore, a status to check if it was WRITTEN is needed */
        if ( wrt_task[i].status == WRITTEN ) {
            strcat(output, "|");
            sprintf(output, "%s", wrt_task[i].label);

            /* Printing space based on distance between 
               previous and current burst time */
            if ( i > 0 ) 
                idx = i-1;
            else 
                idx = 0;
            for ( j = wrt_task[idx].turnaround; j < wrt_task[i].turnaround; ++j ) 
                sprintf(output, "%s", " ");
        }
    }
    sprintf(output, "%s", "|");    /* Closing Bracket for last Process */
    sprintf(output, "%s", "\n");

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
                sprintf(output, "%s", "---");
        }
    }
    sprintf(output, "%s", "\n");

    if ( start_time > 0 )
        sprintf(output, "%s", "0 ");
    sprintf(output, "%d", start_time);
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
                sprintf(output, "%s", " ");
            sprintf(output, " %d ", wrt_task[i].turnaround);
        }
    }
    sprintf(output, "%s", "\n");
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

/* Set all element in arr to val */
void set_arr( int *arr, int size, int val )
{
    int i;
    for ( i = 0; i < size; ++i )
        arr[i] = val;
}

/* Return sum of the burst time of tasks */
int sum_burst( TaskSRTF *tasks, int task_size )
{
    int i;
    int sum = 0;
    for ( i = 0; i < task_size; ++i )
        sum += tasks[i].burst;
    return sum;
}
