#ifndef RESULT_H
#define RESULT_H

void gantt_chart( WriteTask *wrt_task, int wrt_size, int start_time );
double ave_turnaround_time( WriteTask *wrt_task, int wrt_size );
double ave_wait_time( WriteTask *wrt_task, int wrt_size );

#endif
