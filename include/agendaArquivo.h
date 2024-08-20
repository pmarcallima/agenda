#ifndef AGENDAARQUIVO_H 
#define AGENDAARQUIVO_H 

#include "task.h"

int total_bytes(Task *t); 

bool create_task_file(Task *t);

Task delete_task_file(int id);

Task read_task_file(int id);

#endif

