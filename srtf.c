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
    int flag_time, total_burst_time, i, ii;
    int curr_arrival, total_idle_time, idle_time;
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

    total_burst_time = sum_burst_srtf(tasks, task_size);

    /* Selecting process to be allocated in the CPU from time to time */
    i = 0; total_idle_time = 0;
    while ( flag_time < total_burst_time ) {
        if ( hasProcessSRTF( tasks, task_size, flag_time ) == TRUE ) {
            running_task = priority_srtf(flag_time, tasks, task_size); 
            CPU_SRTF(tasks, task_size, running_task, &wrt_task[i], &flag_time);
        } else {
            idle_time = 0;
            curr_arrival = flag_time;
            /* Append spaces everytime when there was no process found */
            while ( hasProcessSRTF( tasks, task_size, flag_time ) == FALSE ) {
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
int hasProcessSRTF( TaskSRTF *tasks, int task_size, int flag_time )
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
TaskSRTF* priority_srtf( int flag_time, TaskSRTF *tasks, int task_size )
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
void CPU_SRTF( TaskSRTF *tasks, int task_size, TaskSRTF *running_task, 
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
        if ( isPreemptSRTF(tasks, task_size, 
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
int isPreemptSRTF( TaskSRTF *tasks, int task_size, int curr_time, TaskSRTF *running_task )
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

/* Return sum of the burst time of tasks */
int sum_burst_srtf( TaskSRTF *tasks, int task_size )
{
    int i;
    int sum = 0;
    for ( i = 0; i < task_size; ++i )
        sum += tasks[i].burst;
    return sum;
}
