#include <hash.h>
#include <stdbool.h>
#include <stdlib.h>
#include <task.h>
#include <time.h>

struct Hash *create_hash(void) {
  struct Hash *h = (struct Hash *)malloc(sizeof(struct Hash));
  h->key = -1;
  for (int i = 0; i < 365; i++)
    h->l[i] = *create_list();

  return h;
}
int hash(int key) { return key % 365; }

bool insert_hash(struct Task *t) {

  struct Hash *h = create_hash();
  time_t ti;
  int dif = difftime(time(&ti), (mktime(&t->data_entrega)));
  dif = dif / 8640;
  if (dif > 0) {
    return insert_list(t, &h->l[hash(dif)]);
  }
  return false;
}
