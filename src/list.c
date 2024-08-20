#include <task.h>
#include <node.h>
#include <list.h>
#include <stdbool.h>
#include <stdlib.h>

struct List* create_list(void)
{
  struct List *l = (struct List*)malloc(sizeof(struct List));

  l->start = create_node();
  return l;
}
bool insert_list(struct Task *t, struct List *l)
{
  struct Node *f = l->start->next;
  while(f->next != NULL)
  {
    f = f->next;
  }
  return insert_node(t, f);

}


