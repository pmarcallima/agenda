#include "agendaArquivo.h"
#include "byte.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

bool insert_index(short id, intptr_t pointer) {
  FILE *file = fopen("id_index.txt", "rb+");
  unsigned char *cont_byte;
  short cont;
  fread(&cont_byte, 2, 1, file);
  cont = byte_to_short(cont_byte);
  fseek(file, 0, SEEK_SET);
  cont++;
  cont_byte = short_to_byte(cont);
  fwrite(&cont_byte, 2, 1, file);
  fseek(file, 0, SEEK_END);
  char lapide = '*';
  fwrite(&lapide, 1, 1, file);
  unsigned char *address = (unsigned char *)malloc(sizeof(intptr_t));
  address = pointer_to_byte(pointer);
  unsigned char *id_byte = short_to_byte(id);
  fwrite(id_byte, 2, 1, file);
  fwrite(&pointer, sizeof(intptr_t), 1, file);
  fclose(file);
  free(address);
  free(id_byte);
  return true;
}

bool delete_index(short id) {
  FILE *file = fopen("id_index.txt", "rb+");
  fseek(file, 0, SEEK_END);
  long end = ftell(file);
  fseek(file, 2, SEEK_SET);
  char lapide;
  unsigned char *tmp = (unsigned char *)malloc(sizeof(short));
  fread(&lapide, 1, 1, file);
  while (ftell(file) != end) {
    if (lapide == ' ')
      fseek(file, 9, SEEK_CUR);
    else {
      fread(tmp, 2, 1, file);
      if (id == byte_to_short(tmp)) {
        unsigned char *ptr_byte = (unsigned char *)malloc(sizeof(intptr_t));
        fread(ptr_byte, 8, 1, file);
        free(tmp);
        fclose(file);
        delete_task_file(byte_to_intptr(ptr_byte));
        free(ptr_byte);
        return true;
      } else
        fseek(file, 7, SEEK_CUR);
    }
  }
  fclose(file);
  free(tmp);
  return false;
}
