
#include <agendaArquivo.h>
#include <byte.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <task.h>

int total_bytes(Task *t) {
  int string_size =
      strlen(t->titulo) + strlen(t->descricao) + sizeof(t->status) + 9;

  return string_size;
}

bool create_task_file(Task *t) {

  FILE *file = fopen("arquivo.txt", "rb+");
  short last_id;
  unsigned char *id = (unsigned char *)malloc(sizeof(unsigned char) * 2);
  fread(id, 2, 1, file);
  last_id = byte_to_int(id);
  free(id);
  last_id++;
  unsigned char *id_bytes = short_to_byte(last_id);
  fseek(file, 0, SEEK_SET);
  fwrite(id_bytes, 2, 1, file);

  fseek(file, 0, SEEK_END);
  char lapide = '*';
  short byte_array_size = total_bytes(t);
  // lapide
  fwrite(&lapide, 1, 1, file);
  fwrite(id_bytes, 2, 1, file);
  fwrite(short_to_byte(byte_array_size), 2, 1, file);
  fwrite(short_to_byte(strlen(t->titulo)), 2, 1, file);
  fwrite(t->titulo, 1, strlen(t->titulo), file);
  fwrite(short_to_byte(strlen(t->descricao)), 2, 1, file);
  fwrite(t->descricao, 1, strlen(t->descricao), file);
  fwrite(short_to_byte(t->data_entrega.tm_mday), 1, 1, file);
  fwrite(short_to_byte(t->data_entrega.tm_mon), 1, 1, file);
  fwrite(short_to_byte(t->data_entrega.tm_year), 2, 1, file);
  fwrite(&t->status, 1, 1, file);

  free(id_bytes);
  fclose(file);

  return false;
}
Task delete_task_file(int id) {
  Task t;
  t.id = 0;
  FILE *file = fopen("arquivo.txt", "rb+");
  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  fseek(file, 0, SEEK_SET);
  char lapide;
  unsigned char *tmp = malloc(sizeof(unsigned char) * 2);
  bool loop = true;
  short string_size;
  short last_id;
  long pos;
  fseek(file, 2, SEEK_SET);
  while (loop) {

    pos = ftell(file);
    lapide = fgetc(file);
    fread(tmp, 2, 1, file);
    last_id = byte_to_short(tmp);
    fread(tmp, 2, 1, file);
    string_size = byte_to_short(tmp);
    if (lapide == ' ') {
      fseek(file, string_size - 1, SEEK_CUR);
    } else {
      if (last_id == id) {
        fread(tmp, 2, 1, file);
        string_size = byte_to_short(tmp);
        char *titulo = malloc(sizeof(char) * string_size + 1);
        fread(titulo, string_size, 1, file);
        titulo[string_size] = '\0';
        strcpy(t.titulo, titulo);
        fread(tmp, 2, 1, file);
        string_size = byte_to_short(tmp);
        char *descricao = malloc(sizeof(char) * string_size + 1);
        fread(descricao, string_size, 1, file);
        descricao[string_size] = '\0';
        strcpy(t.descricao, descricao);
        free(titulo);
        free(descricao);
        fread(tmp, 1, 1, file);
        t.data_entrega.tm_mday = byte_to_short(tmp);
        fread(tmp, 1, 1, file);
        t.data_entrega.tm_mon = byte_to_short(tmp);
        fread(tmp, 2, 1, file);
        t.data_entrega.tm_year = byte_to_short(tmp);
        fread(tmp, 1, 1, file);
        t.status = tmp;
        t.id = last_id;

        fseek(file, pos, SEEK_SET);
        char a = ' ';
        fwrite(&a, 1, 1, file);
        free(tmp);
        fclose(file);
        return t;

      }

      else {

        fseek(file, string_size - 1, SEEK_CUR);
        if (ftell(file) >= size)
          loop = false;
      }
    }
  }
  printf("Task not found\n");
  free(tmp);
  fclose(file);
  return t;
}

Task read_task_file(int id) {
  Task t;
  t.id = 0;
  FILE *file = fopen("arquivo.txt", "rb+");
  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  fseek(file, 0, SEEK_SET);
  char lapide;
  unsigned char *tmp = malloc(sizeof(unsigned char) * 2);
  bool loop = true;
  short string_size;
  short last_id;
  fseek(file, 2, SEEK_SET);
  while (loop) {

    lapide = fgetc(file);
    fread(tmp, 2, 1, file);
    last_id = byte_to_short(tmp);
    fread(tmp, 2, 1, file);
    string_size = byte_to_short(tmp);
    if (lapide == ' ') {
      fseek(file, string_size - 1, SEEK_CUR);
    } else {
      if (last_id == id) {
        fread(tmp, 2, 1, file);
        string_size = byte_to_short(tmp);
        char *titulo = malloc(sizeof(char) * string_size + 1);
        fread(titulo, string_size, 1, file);
        titulo[string_size] = '\0';
        strcpy(t.titulo, titulo);
        fread(tmp, 2, 1, file);
        string_size = byte_to_short(tmp);
        char *descricao = malloc(sizeof(char) * string_size + 1);
        fread(descricao, string_size, 1, file);
        descricao[string_size] = '\0';
        strcpy(t.descricao, descricao);
        free(titulo);
        free(descricao);
        fread(tmp, 1, 1, file);
        t.data_entrega.tm_mday = byte_to_short(tmp);
        fread(tmp, 1, 1, file);
        t.data_entrega.tm_mon = byte_to_short(tmp);
        fread(tmp, 2, 1, file);
        t.data_entrega.tm_year = byte_to_short(tmp);
        fread(tmp, 1, 1, file);
        t.status = tmp;
        t.id = last_id;

        free(tmp);
        fclose(file);
        return t;

      }

      else {

        fseek(file, string_size - 1, SEEK_CUR);
        if (ftell(file) >= size)
          loop = false;
      }
    }
  }
  printf("Task not found\n");
  free(tmp);
  fclose(file);
  return t;
}
