/**
 * Program: main.c
 * Purpose: Runs the visualisation of Gant Chart for Priority Scheduling
 * Author : Calmen Chia
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include "header.h"
#include "file.h"
#include "Task.h"
#include "sort.h"
#include "priority.h"
#include "srtf.h"

pthread_cond_t buffer1_cv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t buffer1_key = PTHREAD_MUTEX_INITIALIZER;
char output[STR];   /* Buffer 2 */

void *start_pp( void *arg )
{
    int size, wrt_size, start_time;
    char turnaround[STR], waiting[STR];
    double ave_turnaround, ave_wait;
    char *fileName = NULL;
    TaskPP *tasks = NULL;
    WriteTask *wrt_task = NULL;

    fileName = (char*)arg;
    /* Read the file and create the tasks */
    tasks = read_task_pp(fileName);

    /* tasks is NULL when there is error reading file */
    /* Process the tasks */
    if ( tasks != NULL ) {
        size = read_file_size(fileName);
        bubble_sort_pp(tasks, size);
        wrt_size = size * WRITE_TASK_LIMIT;
        wrt_task = process_pp(tasks, size, wrt_size);

        /* Check if the first tasks did not start from 0 (Eg: 1,2,3...n) */
        /* If first task did not start from 0, increment the flag_time */
        if ( tasks[0].arrival > 0 ) {
            /* The start time for wrt_task will be arrival time for first task */
            start_time = tasks[0].arrival;
        } else  /* Otherwise, the task is start at time 0 */
            start_time = 0;

        free(tasks); tasks = NULL;
    }

    /* Writing Gantt Chart */
    pthread_mutex_lock(&buffer1_key);
    gantt_chart(wrt_task, wrt_size, start_time);
    printf("\n");
    pthread_mutex_unlock(&buffer1_key);

    /* Calculate Average */
    ave_turnaround = ave_turnaround_time(wrt_task, wrt_size);
    ave_wait = ave_wait_time(wrt_task, wrt_size);

    /* Writing Average Turnaround result */
    sprintf(turnaround, "%s%f", "Average Turnaround Time: ", ave_turnaround);
    strcpy(output, turnaround);
    strcat(output, "\n");

    /* Writing Average Waiting result */
    sprintf(waiting, "%s%f", "Average Waiting Time: ", ave_wait);
    strcat(output, waiting);
    strcat(output, "\n");

    /* Wakeup the parent thread */
    pthread_cond_signal(&buffer1_cv);
    return NULL;
}

void *start_srtf( void *arg )
{
    int size, wrt_size, start_time;
    char turnaround[STR], waiting[STR];
    double ave_turnaround, ave_wait;
    char *fileName = NULL;
    TaskSRTF *tasks = NULL;
    WriteTask *wrt_task = NULL;

    fileName = (char*)arg;
    tasks = read_task_srtf(fileName);

    /* tasks is NULL when there is error reading file */
    if ( tasks != NULL ) {
        size = read_file_size(fileName);
        bubble_sort_srtf(tasks, size);
        wrt_size = size * WRITE_TASK_LIMIT;
        wrt_task = process_srtf(tasks, size, wrt_size);

        /* Check if the first tasks did not start from 0 (Eg: 1,2,3...n) */
        /* If first task did not start from 0, increment the flag_time */
        if ( tasks[0].arrival > 0 ) {
            /* The start time for wrt_task will be arrival time for first task */
            start_time = tasks[0].arrival;
        } else  /* Otherwise, the task is start at time 0 */
            start_time = 0;

        free(tasks); tasks = NULL;
    }

    /* Writing Gantt Chart */
    pthread_mutex_lock(&buffer1_key);
    gantt_chart(wrt_task, wrt_size, start_time);
    printf("\n");
    pthread_mutex_unlock(&buffer1_key);

    /* Calculate Average */
    ave_turnaround = ave_turnaround_time(wrt_task, wrt_size);
    ave_wait = ave_wait_time(wrt_task, wrt_size);

    strcpy(output, "PP: ");
    
    /* Writing Average Turnaround result */
    sprintf(turnaround, "%s%f", "Average Turnaround Time = ", ave_turnaround);
    strcat(output, turnaround);
    strcat(output, ", ");

    /* Writing Average Waiting result */
    sprintf(waiting, "%s%f", "Average Waiting Time = ", ave_wait);
    strcat(output, waiting);
    strcat(output, "\n");

    /* Wakeup the parent thread */
    pthread_cond_signal(&buffer1_cv);
    return NULL;
}

/**
 * Used by parent thread which takes the user input and display
 * the result of output after computation of child threads
 */
void *parent_thread( void *arg )
{
    char *input = NULL;

    input = (char*)arg;
    scanf("%s", input);

    /* Block parent threads and wait for thread 1 and thread 2 */
    if ( strcmp( input, "" ) != 0 )
        pthread_cond_wait(&buffer1_cv, &buffer1_key);   

    printf("%s\n", output);
    return arg;
}

int main()
{
    char input[STR] = "";         /* Buffer 1 */
    pthread_t threads[NUM_THREADS];
    int stop = FALSE;
    int result_code;
    int i;

    while ( stop == FALSE ) {
        printf("INPUT: ");
        scanf("%s", input);

        if ( strcmp( input, "QUIT" ) == 0 )
            stop = TRUE;
        else {
            /* Creating Parent Thread */
            result_code = pthread_create(&threads[0], NULL, parent_thread, input);
            assert(!result_code);

            /* Call thread 1 */
            result_code = pthread_create(&threads[1], NULL, start_pp, &input);
            assert(!result_code);
            
            /* Call thread 2 */
            result_code = pthread_create(&threads[2], NULL, start_srtf, &input);
            assert(!result_code);

            printf("\n");
        }
    }
    return 0;
}
