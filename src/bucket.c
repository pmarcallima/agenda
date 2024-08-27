#include "byte.h"
#include <bucket.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct hash_address_id *create_hash_address_id(void) {
  struct hash_address_id *tmp =
      (struct hash_address_id *)malloc(sizeof(struct hash_address_id));
  tmp->global_depth = -1;
  tmp->element_amount = -1;
  for (int i = 0; i < MAX_DEPTH; i++) {
    tmp->bucket_address[i] = -1;
  }
  return tmp;
}
struct node_bucket *create_node_bucket(void) {
  struct node_bucket *tmp =
      (struct node_bucket *)malloc(sizeof(struct node_bucket));
  tmp->next = NULL;
  tmp->element.address = -1;
  tmp->element.id = -1;
  return tmp;
}
struct bucket *create_bucket(void) {
  struct bucket *tmp = (struct bucket *)malloc(sizeof(struct bucket));
  tmp->list_start = (struct node_bucket **)malloc(sizeof(struct node_bucket *));
  *tmp->list_start = create_node_bucket();

  tmp->local_depth = -1;
  tmp->total_elements = -1;
  return tmp;
}
void update_hash_file(struct hash_address_id h) {
  FILE *hash_file = fopen(FLEX_HASH_FILE, "wb+");
  fwrite(&h.global_depth, 1, 1, hash_file);
  fwrite(&h.total_buckets, 1, 1, hash_file);
  fwrite(&h.element_amount, 1, 1, hash_file);
  int p = pow(2, h.global_depth);
  for (int i = 0; i < p; i++) {
    fwrite(&h.bucket_address[i], 8, 1, hash_file);
  }
  fclose(hash_file);
}
void create_hash_file(void) {
  FILE *hash_file = fopen(FLEX_HASH_FILE, "wb+");
  fwrite(0, 1, 1, hash_file);
  fwrite(0, 1, 1, hash_file);
  char tmp = -1;
  fwrite(&tmp, 1, 1, hash_file);
  // TODO: ver se tem que colocar todos endereços de bucket aqui tambem
  fclose(hash_file);
}
void read_hash_file(struct hash_address_id *h) {
  FILE *hash_file = fopen(FLEX_HASH_FILE, "rb+");
  if (hash_file == NULL) {
    fclose(hash_file);
    create_hash_file();
    return;
  } else
    hash_file = fopen(FLEX_HASH_FILE, "rb+");

  unsigned char *tmp = NULL;

  fread(&h->global_depth, 1, 1, hash_file);
  fread(tmp, 1, 1, hash_file);
  h->element_amount = byte_to_short(tmp);
  fread(&h->total_buckets, 1, 1, hash_file);
  int p = pow(2, h->global_depth);
  for (short i = 0; i < p; i++) {
    fread(&h->bucket_address[i], sizeof(int64_t), 1, hash_file);
  }
  fclose(hash_file);
}
struct bucket *read_all_bucket_file(struct hash_address_id *h) {
  FILE *bucket_file = fopen(BUCKET_FILE, "rb+");
  struct node_bucket *tmp = create_node_bucket();
  struct bucket *buck =
      (struct bucket *)malloc(sizeof(struct bucket) * h->total_buckets);

  for (short i = 0; i < h->total_buckets; i++) {
    fseek(bucket_file, h->bucket_address[i], SEEK_SET);
    fread(&buck[i].local_depth, 1, 1, bucket_file);
    fread(&buck[i].total_elements, 2, 1, bucket_file);
    buck[i].list_start = &tmp;
    int p = pow(2, h->global_depth);
    for (short j = 0; j < p; j++) {
      tmp->next = create_node_bucket();
      fread(&tmp->element.address, 8, 1, bucket_file);
      fread(&tmp->element.id, 2, 1, bucket_file);
      tmp = tmp->next;
    }
  }
  free(tmp);
  fclose(bucket_file);
  return buck;
}

int8_t hash_bucket(int id, short global_depth) {
  int p = pow(2, global_depth);
  p = id % (int)p;
  return abs(p);
}
int8_t hash_bucket_local(int id, short local_depth) {
  int p = pow(2, local_depth);
  p = id % (int)p;
  return abs(p);
}
int double_hash(struct hash_address_id *h) {
  if (h->global_depth >= 8)
    return 0;
  else {
    h->global_depth++;
    int q1 = (int)pow(2, h->global_depth - 1);
    int q2 = (int)pow(2, h->global_depth);
    long new_address[q2];
    int i = 0;
    while (i < q1) {
      new_address[i] = h->bucket_address[i];
    }
    while (i < q2) {
      new_address[i] = h->bucket_address[i - q1];
    }
    for (i = 0; i < q2; i++)
      h->bucket_address[i] = new_address[i];
    return 1;
  }
}

int insert_bucket_file(struct address_id new) {

  struct hash_address_id *h = create_hash_address_id();
  read_hash_file(h);
  long address = h->bucket_address[hash_bucket(new.id, h->global_depth)];
  FILE *bucket_file = fopen(BUCKET_FILE, "rb+");
  fseek(bucket_file, address, SEEK_SET);
  struct bucket *b = create_bucket();
  fread(&b->local_depth, 1, 1, bucket_file);
  fread(&b->total_elements, 2, 1, bucket_file);
  struct node_bucket *free_ptr = *b->list_start;
  for (int i = 0; i < b->total_elements; i++) {
    free_ptr->next = create_node_bucket();
    free_ptr = free_ptr->next;
    fread(&free_ptr->element.address, 8, 1, bucket_file);
    fread(&free_ptr->element.id, 2, 1, bucket_file);
  }

  if (b->total_elements < 24) {
    b->total_elements++;
    fseek(bucket_file, -2, SEEK_CUR);
    fwrite(&b->total_elements, 2, 1, bucket_file);
    fseek(bucket_file, ELEMENT_SIZE * b->total_elements, SEEK_CUR);
    fwrite(&new.address, 8, 1, bucket_file);
    fwrite(&new.id, 2, 1, bucket_file);
    free(h);
    free(b);
    fclose(bucket_file);
    return 1;
  } else {
    if (b->local_depth >= h->global_depth) {
      double_hash(h);
    }
    if (b->local_depth < h->global_depth) {

      fseek(bucket_file, address, SEEK_SET);
      struct bucket *new_b = create_bucket();
      new_b->local_depth = b->local_depth++;

      fwrite(&new_b->local_depth, 1, 1, bucket_file);
      fwrite(0, 1, 1, bucket_file);

      struct bucket *old_b = create_bucket();
      long new_address = address * pow(2, b->local_depth);
      fseek(bucket_file, new_address, SEEK_SET);
      old_b->local_depth = h->global_depth;
      fwrite(&old_b->local_depth, 1, 1, bucket_file);
      fwrite(0, 1, 1, bucket_file);
      fclose(bucket_file);

      int start = hash_bucket_local(new.id, b->local_depth);
      int offset = (int)pow(2, b->local_depth);
      int max = (int)pow(2, h->global_depth);
      bool flag = false;
      for (int i = start; i < max; i += offset) {
        if (flag)
          h->bucket_address[i] = i * 256;
        flag = true;
      }
    }
    update_hash_file(*h);
    free(h);

    struct node_bucket *tmp = *b->list_start;
    int total_elements = b->total_elements;
    for (int i = 0; i < total_elements; i++) {
      tmp = tmp->next;
      insert_bucket_file(tmp->element);
    }
    free(b);
  }
  return 0;
}

void insert_hash_file(struct hash_address_id h) {}
