#ifndef TASK
#define TASK

#include "header.h" /* To allow the usage of STR */

/* Array of Task to be created */
typedef struct TaskPriority {
    int arrival;        /* Arrival time of the task */
    int burst;          /* Burst time of the task */
    int priority;       /* Lower integer indicates higher priority */
    char label[STR];    /* Eg: P1, P2 ... Pn */
    int status;         /* Status of the task in CPU (Eg: Done) */
} TaskPriority;

/* Array of Task to be created */
typedef struct TaskSRTF {
    int arrival;        /* Arrival time of the task */
    int burst;          /* Burst time of the task */
    char label[STR];    /* Eg: P1, P2 ... Pn */
    int status;         /* Status of the task in CPU (Eg: Done) */
} TaskSRTF;

/* Array of Task use to write the Gantt Chart */
typedef struct WriteTask {
    int arrival;        /* Use to calculate waiting time */
    int turnaround;
    char label[STR];
    int status;         /* Initial value is -1 */
} WriteTask;

#endif
