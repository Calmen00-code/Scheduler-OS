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
pthread_cond_t buffer2_cv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t buffer1_key = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t buffer2_key = PTHREAD_MUTEX_INITIALIZER;
char buffer2[1][STR];   

void *start_pp( void *arg )
{
    int size, wrt_size, start_time;
    char turnaround[STR], waiting[STR];
    double ave_turnaround, ave_wait;
    char *fileName = NULL;
    TaskPP *tasks = NULL;
    WriteTask *wrt_task = NULL;

    fileName = (char*)arg;

    /* NOTE: Consumer for buffer 1 */
    /* ASSERTION: Consumer will never consume until buffer is produced */
    pthread_mutex_lock(&buffer1_key);
    tasks = read_task_pp(fileName);
    pthread_mutex_unlock(&buffer1_key);

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

        /* Writing Gantt Chart */
        /* NOTE: Consumer for buffer 1 */
        /* ASSERTION: Consumer will never consume until buffer is produced */
        pthread_mutex_lock(&buffer1_key);
        printf("Priority:\n");
        gantt_chart(wrt_task, wrt_size, start_time);
        printf("\n");
        pthread_mutex_unlock(&buffer1_key);

        /**
         * NOTE: Producer of buffer 2
         * ASSERTION: Consumer buffer 2 will never consume until buffer 2 is produced
         */
        /* Producer for buffer 2 starts here */
        pthread_mutex_lock(&buffer2_key);

        /* Calculate Average */
        ave_turnaround = ave_turnaround_time(wrt_task, wrt_size);
        ave_wait = ave_wait_time(wrt_task, wrt_size);

        /* Writing Average Turnaround result */
        sprintf(turnaround, "%s%f", "Average Turnaround Time: ", ave_turnaround);
        strcpy(buffer2[1], turnaround);
        strcat(buffer2[1], "\n");

        /* Writing Average Waiting result */
        sprintf(waiting, "%s%f", "Average Waiting Time: ", ave_wait);
        strcat(buffer2[1], waiting);
        strcat(buffer2[1], "\n");

        pthread_mutex_unlock(&buffer2_key);
        /** End of producer buffer 2 */

        /* Wakeup the parent thread after all data retrieved for buffer 2 */
        pthread_cond_signal(&buffer1_cv);

        /* Block the thread and wait for parent thread to consume buffer 2 */
        pthread_cond_wait(&buffer2_cv, &buffer2_key);

        free(tasks); tasks = NULL;
    }
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

    /* NOTE: Consumer for buffer 1 */
    /* ASSERTION: Consumer will never consume until buffer is produced */
    pthread_mutex_lock(&buffer1_key);
    tasks = read_task_srtf(fileName);
    pthread_mutex_unlock(&buffer1_key);

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

        /* Writing Gantt Chart */
        /* NOTE: Consumer for buffer 1 */
        /* ASSERTION: Consumer buffer 1 will never consume until buffer 1 is produced */
        pthread_mutex_lock(&buffer1_key);
        printf("SRTF:\n");
        gantt_chart(wrt_task, wrt_size, start_time);
        printf("\n");
        pthread_mutex_unlock(&buffer1_key);

        /**
         * NOTE: Producer of buffer 2
         * ASSERTION: Consumer buffer 2 will never consume until buffer 2 is produced
         */
        /* Producer for buffer 2 starts here */
        pthread_mutex_lock(&buffer2_key);

        /* Calculate Average */
        ave_turnaround = ave_turnaround_time(wrt_task, wrt_size);
        ave_wait = ave_wait_time(wrt_task, wrt_size);

        strcpy(buffer2[0], "SRTF: ");
        
        /* Writing Average Turnaround result */
        sprintf(turnaround, "%s%f", "Average Turnaround Time = ", ave_turnaround);
        strcat(buffer2[0], turnaround);
        strcat(buffer2[0], ", ");

        /* Writing Average Waiting result */
        sprintf(waiting, "%s%f", "Average Waiting Time = ", ave_wait);
        strcat(buffer2[0], waiting);
        strcat(buffer2[0], "\n");

        pthread_mutex_unlock(&buffer2_key);
        /** End of producer buffer 2 */

        /* Wakeup the parent thread after all data retrieved for buffer 2 */
        pthread_cond_signal(&buffer1_cv);

        /* Block the thread and wait for parent thread to consume buffer 2 */
        pthread_cond_wait(&buffer2_cv, &buffer2_key);

        free(tasks); tasks = NULL;
    }
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

    /* Producer for buffer 1 */
    /** 
     * Child threads will not write to buffer 1 
     * until this buffer is filled
     */
    pthread_mutex_lock(&buffer1_key);
    printf("INPUT: ");
    scanf("%s", input);
    pthread_mutex_unlock(&buffer1_key);

    /* Block parent threads and wait for thread 1 and thread 2 */
    pthread_cond_wait(&buffer1_cv, &buffer1_key);   

    /* Consumer for buffer 2 */
    /**
     * This thread will not print (consume) buffer 2 until 
     * child threads the producer of buffer 2 writes into the buffer
     */
    pthread_mutex_lock(&buffer2_key);
    printf("%s\n", buffer2[0]);
    /* Wakeup child thread after consuming buffer 2 */
    pthread_cond_signal(&buffer2_cv);   
    pthread_mutex_unlock(&buffer2_key);
    return NULL;
}

int main()
{
    char buffer1[1][STR];
    pthread_t threads[NUM_THREADS];
    int result_code;
    int stop = FALSE;

    while ( stop == FALSE ) {
        if ( strcmp( buffer1[0], "QUIT" ) == 0 )
            stop = TRUE;
        else {
            /* Creating Parent Thread */
            result_code = pthread_create(&threads[0], NULL, parent_thread, buffer1[0]);
            assert(!result_code);

            /* Call thread 1 */
            result_code = pthread_create(&threads[1], NULL, start_pp, buffer1[0]);
            assert(!result_code);
                    
            /* Call thread 2 */
            result_code = pthread_create(&threads[2], NULL, start_srtf, buffer1[0]);
            assert(!result_code);
        }
    }

    /* Wait for all threads to terminate */
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);

    return 0;
}
