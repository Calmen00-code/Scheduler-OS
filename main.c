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
    char *fileName = NULL;
    int size, wrt_size;
    Task *tasks = NULL;
    WriteTask *wrt_task = NULL;
    char turnaround[STR], waiting[STR];
    double ave_turnaround, ave_wait;

    fileName = (char*)arg;
    tasks = read_task(fileName);

    /* tasks is NULL when there is error reading file */
    if ( tasks != NULL ) {
        size = read_file_size(fileName);
        bubble_sort(tasks, size);
        wrt_size = size * WRITE_TASK_LIMIT;
        wrt_task = process_srtf(tasks, size, wrt_size);
        free(tasks); tasks = NULL;
    }

    /* Writing Gantt Chart */
    gantt_chart_srtf(wrt_task, wrt_size, start_time, output);
    strcat(output, "\n");

    /* Calculate Average */
    ave_turnaround = ave_turnaround_time_srtf(wrt_task, wrt_size);
    ave_wait = ave_wait_time_srtf(wrt_task, wrt_size);

    /* Writing Average Turnaround result */
    sprintf(turnaround, "%s%f", "Average Turnaround Time: ", ave_turnaround);
    strcat(output, turnaround);
    strcat(output, "\n");

    /* Writing Average Waiting result */
    sprintf(waiting, "%s%f", "Average Waiting Time: ", ave_wait);
    strcat(output, waiting);
    strcat(output, "\n");
    return task;
}

void *start_srtf( void *arg )
{
    char *fileName = NULL;
    int size, wrt_size;
    Task *tasks = NULL;
    WriteTask *wrt_task = NULL;
    char turnaround[STR], waiting[STR];
    double ave_turnaround, ave_wait;

    fileName = (char*)arg;
    tasks = read_task(fileName);

    /* tasks is NULL when there is error reading file */
    if ( tasks != NULL ) {
        size = read_file_size(fileName);
        bubble_sort(tasks, size);
        wrt_size = size * WRITE_TASK_LIMIT;
        wrt_task = process_srtf(tasks, size, wrt_size);
        free(tasks); tasks = NULL;
    }

    /* Writing Gantt Chart */
    gantt_chart_srtf(wrt_task, wrt_size, start_time, output);
    strcat(output, "\n");

    /* Calculate Average */
    ave_turnaround = ave_turnaround_time_srtf(wrt_task, wrt_size);
    ave_wait = ave_wait_time_srtf(wrt_task, wrt_size);

    /* Writing Average Turnaround result */
    sprintf(turnaround, "%s%f", "Average Turnaround Time: ", ave_turnaround);
    strcat(output, turnaround);
    strcat(output, "\n");

    /* Writing Average Waiting result */
    sprintf(waiting, "%s%f", "Average Waiting Time: ", ave_wait);
    strcat(output, waiting);
    strcat(output, "\n");
    return task;
}

/* Used by parent thread which takes the user input */
void *user_input( void *arg )
{
    char *input = NULL;

    input = (char*)arg;
    scanf("%s", input);

    /* Block parent threads and wait for thread 1 and thread 2 */
    pthread_cond_wait(&buffer1_cv, buffer1_key);   
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
            result_code = pthread_create(&thread[0], NULL, user_input, input);
            assert(!result_code);

            /* Call thread 1 */
            pthread_mutex_lock(&buffer1_key);
            result_code = pthread_create(&thread[1], NULL, start_pp, &input);
            assert(!result_code);
            pthread_mutex_unlock(&buffer1_key);
            
            /* Call thread 2 */
            pthread_mutex_lock(&buffer1_key);
            result_code = pthread_create(&thread[2], NULL, start_srtf, &input);
            assert(!result_code);
            pthread_mutex_unlock(&buffer1_key);

            /* Wakeup the parent thread */
            pthread_cond_signal(&buffer1_cv, buffer1_key);

            printf("\n");
        }
    }
    return 0;
}
