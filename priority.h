#ifndef SCHEDULE_PP
#define SCHEDULE_PP

#include "Task.h"

WriteTask* process_pp( TaskPP *task, int task_size, int wrt_size );
void CPU_PP( TaskPP *tasks, int task_size, TaskPP *running_task, WriteTask *wrt_task, int *flag_time );
int hasProcessPP( TaskPP* tasks, int task_size, int flag_time );
int sum_burst_pp( TaskPP *tasks, int task_size );
TaskPP* priority_pp( int flag_time, TaskPP *tasks, int task_size );
int isPreemptPP( TaskPP *tasks, int task_size, int curr_time, TaskPP *running_task );
void gantt_chart( WriteTask *wrt_task, int wrt_size, int start_time );
double ave_turnaround_time( WriteTask *wrt_task, int wrt_size );
double ave_wait_time( WriteTask *wrt_task, int wrt_size );

#endif
