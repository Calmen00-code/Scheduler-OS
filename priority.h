#ifndef SCHEDULE_PP
#define SCHEDULE_PP

#include "Task.h"

WriteTask* process_pp( Task *task, int task_size, int wrt_size );
void CPU( Task *tasks, int task_size, Task *running_task, WriteTask *wrt_task, int *flag_time );
int hasProcess( Task* tasks, int task_size, int flag_time );
int sum_burst( Task *tasks, int task_size );
void set_arr( int *arr, int size, int val );
Task* priority( int flag_time, Task *tasks, int task_size );
int next_preempt( Task *tasks, int task_size, Task *running_task, int *preempt_idx );
int isPreempt( Task *tasks, int task_size, int curr_time, Task *running_task );
void gantt_chart( WriteTask *wrt_task, int wrt_size, int start_time );
double ave_turnaround_time( WriteTask *wrt_task, int wrt_size );
double ave_wait_time( WriteTask *wrt_task, int wrt_size );

#endif
