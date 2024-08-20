#ifndef TASK_H 
#define TASK_H 

#include <stdbool.h>
#include <time.h>

#define MAX_TAM 100
typedef struct Task {
  int id;
  char titulo[MAX_TAM];
  char descricao[MAX_TAM];
  struct tm data_entrega;
  bool status;

} Task;
#endif // TASK_H 
