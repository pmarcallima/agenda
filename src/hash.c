#include "node.h"
#include <hash.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <task.h>
#include <time.h>

struct Hash *create_hash(void) {
  struct Hash *h = (struct Hash *)malloc(sizeof(struct Hash));
  h->key = -1;

  for (int i = 0; i < 365; i++) {
    h->l[i] = create_list();
  }
  return h;
}
int hash(int key) { return key % 365; }

bool insert_hash(struct Hash *h, struct Task *t) {

  time_t ti;
  time(&ti);
  struct tm *tmp = gmtime(&ti);
  tmp->tm_sec = 0;
  tmp->tm_min = 0;
  tmp->tm_hour = 0;
  int dif = difftime((mktime(&t->data_entrega)), (mktime(tmp)));
  dif = dif / (60 * 60 * 24);

  printf("dif = %d", hash(dif));
  if (dif > 0) {
    insert_list(*t, h->l[hash(dif)]);
    // printf("%s", h->l[hash(dif)].start->next->t->titulo);
    print_hash(h);
    free(h);
    return true;
  }
  free(h);
  return false;
}

void print_hash(struct Hash *h) {
  struct Node *tmp;
  tmp = h->l[hash(10)]->start->next;
  while (tmp != NULL) {

    printf("%s", tmp->t->titulo);
    tmp = tmp->next;
  }
}
