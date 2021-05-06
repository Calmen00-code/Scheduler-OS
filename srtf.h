#ifndef SCHEDULE_SRTF
#define SCHEDULE_SRTF

#include "Task.h"

WriteTask* process_srtf( TaskSRTF *task, int task_size, int wrt_size );
void CPU_SRTF( TaskSRTF *tasks, int task_size, TaskSRTF *running_task, WriteTask *wrt_task, int *flag_time );
int hasProcessSRTF( TaskSRTF *tasks, int task_size, int flag_time );
int sum_burst_srtf( TaskSRTF *tasks, int task_size );
void set_arr( int *arr, int size, int val );
TaskSRTF* priority_srtf( int flag_time, TaskSRTF *tasks, int task_size );
int isPreemptSRTF( TaskSRTF *tasks, int task_size, int curr_time, TaskSRTF *running_task );

#endif
