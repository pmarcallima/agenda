#ifndef HASH_H
#define HASH_H

#define DAY_QNT_MAX 365
#include "list.h"

struct Hash {
  int key;
  struct List *l[DAY_QNT_MAX];
};

struct Hash *create_hash(void);

bool insert_hash(struct Hash *h, struct Task *t);

void print_hash(struct Hash *h);

#endif
