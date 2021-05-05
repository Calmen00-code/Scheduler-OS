/**
 * Program: file.c
 * Purpose: Reading the file that contains Tasks/Processes and store
 *          the task into the Task Array
 * Author : Calmen Chia
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file.h"

/**
 * Read the content of the file and assign all the fields in Task struct
 * This function initialises every fields of the Task Array
 * Reading the file for Priority Scheduling
 */
Task* read_task_pp( char filename[] )
{
    FILE *readPtr = NULL;
    Task *tasks = NULL;
    int i, size;
    int arrival_time, burst_time, priority;
    char process_label[STR];
    char p_label = 'P';

    readPtr = fopen(filename, "r");
    if ( readPtr == NULL ) {
        perror("Error while reading file");
    }
    else {
        /* Determining the size of the Task Array */
        size = read_file_size( filename );  
        tasks = calloc(sizeof(Task), size);

        i = 0;
        while ( fscanf(readPtr, "%d %d %d\n", &arrival_time, 
                       &burst_time, &priority ) != EOF )
        {
            tasks[i].arrival = arrival_time;
            tasks[i].burst = burst_time;
            tasks[i].priority = priority;
            sprintf(process_label, "%c%d", p_label, i+1);
            strcpy(tasks[i].label, process_label);
            ++i;
        }
        fclose(readPtr); readPtr = NULL;
    }
    return tasks;
} 

/**
 * Read the content of the file and assign all the fields in Task struct
 * This function initialises every fields of the Task Array
 * Reading the file for Shortest Remaining Job Time Scheduling
 */
Task* read_task_srtf( char filename[] )
{
    FILE *readPtr = NULL;
    Task *tasks = NULL;
    int i, size;
    int arrival_time, burst_time;
    char process_label[STR];
    char p_label = 'P';

    readPtr = fopen(filename, "r");
    if ( readPtr == NULL ) {
        perror("Error while reading file");
    }
    else {
        /* Determining the size of the Task Array */
        size = read_file_size( filename );  
        tasks = calloc(sizeof(Task), size);

        i = 0;
        while ( fscanf(readPtr, "%d %d\n", &arrival_time, 
                       &burst_time ) != EOF )
        {
            tasks[i].arrival = arrival_time;
            tasks[i].burst = burst_time;
            sprintf(process_label, "%c%d", p_label, i+1);
            strcpy(tasks[i].label, process_label);
            ++i;
        }
        fclose(readPtr); readPtr = NULL;
    }
    return tasks;
} 

/**
 * Read the number of lines in a file
 */
int read_file_size( char filename[] )
{
    FILE *readPtr = NULL;
    int line = 0;
    char str[STR];

    readPtr = fopen(filename, "r");
    if ( readPtr == NULL ) {
        perror("Error while reading file");
    }
    else {
        /* Reading stops when the pointer reaches the end of file */
        while ( fgets( str, STR, readPtr ) != NULL )
            ++line;
        fclose(readPtr); readPtr = NULL;
    }
    return line;
}
