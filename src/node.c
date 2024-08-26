#include "task.h"
#include <node.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Node *create_node(void) {
  struct Node *n;
  n = (struct Node *)malloc(sizeof(struct Node));
  n->next = NULL;
  n->t = create_task();
  return n;
}

bool insert_node(struct Task i, struct Node *f) {
  struct Node *new_node = create_node();
  f->next = new_node;
  f = f->next;
  f->t = &i;
  printf("titulo: %s, descricao: %s, data: %d - %d - %d", f->t->titulo,
         f->t->descricao, f->t->data_entrega.tm_mday, f->t->data_entrega.tm_mon,
         f->t->data_entrega.tm_year);
  return true;
}
