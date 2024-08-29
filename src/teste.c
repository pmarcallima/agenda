#include <agendaArquivo.h>
#include <bucket.h>
#include <hash.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
/*
void read_and_print_bucket_file(void) {
  FILE *bucket_file = fopen(BUCKET_FILE, "rb");
  if (bucket_file == NULL) {
    perror("Failed to open file for reading");
    return;
  }

  // Ler e imprimir a estrutura bucket
  struct bucket b;

  // Ler local_depth
  if (fread(&b.local_depth, sizeof(b.local_depth), 1, bucket_file) != 1) {
    perror("Failed to read local_depth");
    fclose(bucket_file);
    return;
  }
  printf("Local depth: %d\n", b.local_depth);

  // Ler total_elements
  if (fread(&b.total_elements, sizeof(b.total_elements), 1, bucket_file) != 1) {
    perror("Failed to read total_elements");
    fclose(bucket_file);
    return;
  }
  printf("Total elements: %d\n", b.total_elements);

  // Ler e imprimir cada node_bucket
  struct node_bucket *current = NULL;
  for (int i = 0; i < b.total_elements; i++) {
    struct node_bucket *node = malloc(sizeof(struct node_bucket));
    if (!node) {
      perror("Failed to allocate memory for node_bucket");
      fclose(bucket_file);
      return;
    }

    if (fread(&node->element.address, sizeof(node->element.address), 1,
              bucket_file) != 1 ||
        fread(&node->element.id, sizeof(node->element.id), 1, bucket_file) !=
            1) {
      perror("Failed to read node_bucket element");
      free(node);
      fclose(bucket_file);
      return;
    }

    // Imprimir o conteúdo do node_bucket
    printf("Node bucket %d:\n", i);
    printf("  Address: %ld\n", node->element.address);
    printf("  ID: %d\n", node->element.id);

    // Configurar o ponteiro next
    node->next = current;
    current = node;
  }

  // A lista está em ordem inversa; inverta-a para impressão correta
  struct node_bucket *reversed_list = NULL;
  while (current) {
    struct node_bucket *next = current->next;
    current->next = reversed_list;
    reversed_list = current;
    current = next;
  }

  printf("Node bucket list:\n");
  current = reversed_list;
  while (current) {
    printf("Node bucket:\n");
    printf("  Address: %ld\n", current->element.address);
    printf("  ID: %d\n", current->element.id);
    current = current->next;
  }

  // Liberar a memória alocada
  current = reversed_list;
  while (current) {
    struct node_bucket *next = current->next;
    free(current);
    current = next;
  }

  fclose(bucket_file);
}

int main(void) {
  read_and_print_bucket_file();
  return 0;
}*/

void read_and_print_hash_file(void) {
  FILE *hash_file = fopen(FLEX_HASH_FILE, "rb");
  if (hash_file == NULL) {
    perror("Failed to open file for reading");
    return;
  }

  // Ler e imprimir o primeiro valor (int8_t)
  int8_t value8;
  if (fread(&value8, sizeof(value8), 1, hash_file) == 1) {
    printf("First value (int8_t): %d\n", value8);
  } else {
    perror("Failed to read first value");
    fclose(hash_file);
    return;
  }

  // Ler e imprimir o segundo valor (int8_t)
  if (fread(&value8, sizeof(value8), 1, hash_file) == 1) {
    printf("Second value (int8_t): %d\n", value8);
  } else {
    perror("Failed to read second value");
    fclose(hash_file);
    return;
  }

  // Ler e imprimir os 256 valores (int64_t)
  int64_t value64;
  for (int i = 0; i < value8; i++) {
    if (fread(&value64, 8, 1, hash_file) == 1) {
      printf("Value %d (int64_t): %ld\n", i, value64);
    } else {
      perror("Failed to read int64_t value");
      fclose(hash_file);
      return;
    }
  }

  fclose(hash_file);
}

int main(void) {
  read_and_print_hash_file();
  return 0;
}

/*
int main(void) {
  struct Task *t = malloc(sizeof(struct Task));

  t->data_entrega.tm_year = 2024 - 1900;
  t->data_entrega.tm_mday = 30;
  t->data_entrega.tm_mon = 7;
  t->titulo[0] = 'c';
  t->titulo[1] = '\0';
  t->descricao[0] = 'a';
  t->descricao[1] = '\0';
  t->data_entrega.tm_sec = 0;
  t->data_entrega.tm_min = 0;
  t->data_entrega.tm_hour = 0;
  struct hash_address_id *h = create_hash_address_id();
  read_hash_file(h);
  struct bucket *b = read_all_bucket_file(h);
  for (int i = 0; i < 300; i++) {
    t->id = i;
    create_task_file(t);
  }
  free(t);
  printf("Total buckets: %d\n", h->total_buckets);
  for (int i = 0; i < h->total_buckets; i++) {
    // printf("Bucket total elements: %d :\n", b[i].total_elements);
    struct node_bucket *nb = b[i].list_start->next;
    while (nb != NULL) {
      printf("  Element ID: %d\n", nb->element.id);
      nb = nb->next;
    }
  }
  free(h);
  free(b);
}
/*
int main(void) {

  struct Task *t = malloc(sizeof(struct Task));
  t->data_entrega.tm_year = 2024 - 1900;
  t->data_entrega.tm_mday = 30;
  t->data_entrega.tm_mon = 7;
  t->titulo[0] = 'c';
  t->titulo[1] = '\0';
  t->descricao[0] = 'a';
  t->descricao[1] = '\0';
  t->data_entrega.tm_sec = 0;
  t->data_entrega.tm_min = 0;
  t->data_entrega.tm_hour = 0;
  struct Hash *h = create_hash();
  insert_hash(h, t);
  free(t);
}*/
