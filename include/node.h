#ifndef NODE_H
#define NODE_H

#include <stdbool.h>
#include <task.h>

struct Node {
  struct Node *next;
  struct Task *t;
};

struct Node *create_node(void);

bool insert_node(struct Task t, struct Node *n);

#endif
