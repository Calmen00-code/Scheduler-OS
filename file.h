#ifndef FILE_H
#define FILE_H

#include "Task.h"

TaskPP* read_task_pp( char filename[] );
TaskSRTF* read_task_srtf( char filename[] );
int read_file_size( char filename[] );

#endif
