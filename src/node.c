#include "task.h"
#include <node.h>
#include <stdbool.h>
#include <stdlib.h>


struct Node* create_node(void)
{
  struct Node *n;
  n = (struct Node*)malloc(sizeof(struct Node*));
  n->next = NULL;
  //new_task(n->t);
  return n;
}

bool insert_node(struct Task *i, struct Node *n)
{
  n->next = create_node();
  n = n->next;
  n->t = i;
  return true;

}
