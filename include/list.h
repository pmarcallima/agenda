#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <task.h>

struct List {
  struct Node *start;
};
struct List *create_list(void);

bool insert_list(struct Task t, struct List *l);

#endif
