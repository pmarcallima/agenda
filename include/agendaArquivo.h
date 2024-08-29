#ifndef AGENDAARQUIVO_H
#define AGENDAARQUIVO_H

#include "task.h"
#include <stdint.h>

int total_bytes(Task *t);

bool create_task_file(Task *t);

Task delete_task_file(int id, int64_t address);

Task read_task_file(int id);

#endif
