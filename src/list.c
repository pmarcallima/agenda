#include <list.h>
#include <node.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <task.h>

struct List *create_list(void) {
  struct List *l = (struct List *)malloc(sizeof(struct List));

  l->start = create_node();
  return l;
}
bool insert_list(struct Task t, struct List *l) {
  struct Node *f = l->start;
  while (f->next != NULL) {
    f = f->next;
  }
  return insert_node(t, f);
}
