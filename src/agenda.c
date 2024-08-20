

#include <agenda.h>
#include <agendaArquivo.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define FEITO true
#define PENDENTE false

void mostrar_tarefa(Task *t) {
  char buffer[80];
  strftime(buffer, 80, "%d/%m/%Y", &t->data_entrega);
  printf("A task: %s\nDe descricao %s\nDe data %s \n %b \nfoi criada com "
         "sucesso!! \n",
         t->titulo, t->descricao, buffer, t->status);
}
bool verificar_bissexto(int ano) {

  if (ano % 400 == 0)
    return true;
  else if (ano % 4 == 0 && ano % 100 != 0)
    return true;
  else
    return false;
}
bool verificar_data(int dia, int mes, int ano) {
  bool resp = false;
  mes++;
  if (mes <= 12 && mes >= 1 && ano > 0) {
    bool bissexto = verificar_bissexto(ano);
    if (dia <= 31 && dia > 0 &&
        (mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 ||
         mes == 10 || mes == 12))
      resp = true;
    else if (dia <= 30 && dia > 0 && (mes == 4 || mes == 9 || mes == 11))
      resp = true;
    else if (bissexto == true && dia <= 29 && dia > 0 && mes == 2)
      resp = true;
    else if (dia <= 28 && dia > 0 && mes == 2)
      resp = true;
  }
  return resp;
}
Task *criar_tarefa(Task *task) {
  bool loop = true;
  int dia, mes, ano;

  printf("Crie uma task");
  printf("\nDigite o titulo dessa task\n");
  fgets(task->titulo, 99, stdin);
  printf("\nDigite a descricao dessa task\n");
  fgets(task->descricao, 99, stdin);
  printf("\nDigite a data de entrega seguindo o formato dd mm aaaa\n");
  while (loop) {
    // a struct tm conta a distancia de janeiro por isso -1
    // a struct tm conta anos a partir de 1900 por isso - 1900
    scanf("%d %d %d", &dia, &mes, &ano);
    getchar();
    ano = ano - 1900;
    mes = mes - 1;
    bool resp = verificar_data(dia, mes, ano);
    if (resp == false)
      printf("Digite uma data valida!\n");
    else
      loop = false;
  }

  task->data_entrega.tm_mday = dia;
  task->data_entrega.tm_mon = mes;
  task->data_entrega.tm_year = ano;
  mostrar_tarefa(task);
  create_task_file(task);

  return task;
}
