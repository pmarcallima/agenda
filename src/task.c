#include <stdlib.h>
#include <task.h>

struct Task *create_task(void) {
  struct Task *tmp = (struct Task *)malloc(sizeof(struct Task));
  return tmp;
}
