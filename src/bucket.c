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
  for (int i = 0; i < 256; i++) {
    tmp->bucket_address[i] = i * 256;
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
  tmp->list_start = (struct node_bucket *)malloc(sizeof(struct node_bucket));
  tmp->list_start = create_node_bucket();

  tmp->local_depth = -1;
  tmp->total_elements = 0;
  return tmp;
}
void update_bucket_file(struct bucket b, int64_t address) {
  FILE *bucket_file = fopen(BUCKET_FILE, "rb+");
  fseek(bucket_file, address, SEEK_SET);

  fwrite(&b.local_depth, 1, 1, bucket_file);
  fwrite(&b.total_elements, 1, 1, bucket_file);

  struct node_bucket *nb = b.list_start;
  int8_t zero = 0;

  for (int i = 0; i < b.total_elements + 1; i++) {
    fwrite(&zero, 10, 1, bucket_file);
  }
  fseek(bucket_file, address + 2, SEEK_SET);
  for (int i = 0; i < b.total_elements; i++) {
    fwrite(&nb->element.address, sizeof(int64_t), 1, bucket_file);
    fwrite(&nb->element.id, sizeof(int16_t), 1, bucket_file);
  }
  fclose(bucket_file);
}
void update_hash_file(struct hash_address_id h) {
  FILE *hash_file = fopen(FLEX_HASH_FILE, "wb+");
  fwrite(&h.global_depth, 1, 1, hash_file);
  fwrite(&h.total_buckets, 1, 1, hash_file);
  int p = pow(2, h.global_depth);
  for (int i = 0; i < p; i++) {
    fwrite(&h.bucket_address[i], 8, 1, hash_file);
  }
  fclose(hash_file);
}

void create_bucket_file(void) {
  FILE *bucket_file = fopen(BUCKET_FILE, "wb+");
  if (bucket_file == NULL) {
    perror("Failed to open file for writing");
    return;
  }

  int8_t tmp = 1;
  fwrite(&tmp, sizeof(tmp), 1, bucket_file);

  int16_t zero = 0;
  fwrite(&zero, sizeof(zero), 1, bucket_file);

  int8_t zero_arr[256] = {0}; // Array of 256 bytes, all initialized to 0
  fwrite(zero_arr, sizeof(zero_arr), 1, bucket_file);
  fclose(bucket_file);
}
void create_hash_file(void) {
  FILE *hash_file = fopen(FLEX_HASH_FILE, "wb+");
  if (hash_file == NULL) {
    perror("Failed to open file for writing");
    return;
  }

  int8_t tmp = 1;
  fwrite(&tmp, sizeof(tmp), 1, hash_file);

  tmp = 2;
  fwrite(&tmp, sizeof(tmp), 1, hash_file);

  int64_t zero64;
  for (int i = 0; i < 256; i++) {
    zero64 = i * 256;
    fwrite(&zero64, sizeof(zero64), 1, hash_file);
  }
  fclose(hash_file);
}
void read_hash_file(struct hash_address_id *h) {
  FILE *hash_file = fopen(FLEX_HASH_FILE, "rb+");
  if (hash_file == NULL) {
    create_hash_file();
    hash_file = fopen(FLEX_HASH_FILE, "rb+");
    if (hash_file == NULL) {
      perror("Failed to open file after creation");
      return;
    }
  }

  fread(&h->global_depth, sizeof(h->global_depth), 1, hash_file);
  fread(&h->total_buckets, sizeof(h->total_buckets), 1, hash_file);
  for (short i = 0; i < h->total_buckets; i++) {
    fread(&h->bucket_address[i], sizeof(int64_t), 1, hash_file);
  }

  fclose(hash_file);
}
struct bucket *read_all_bucket_file(struct hash_address_id *h) {
  FILE *bucket_file = fopen(BUCKET_FILE, "rb+");
  if (bucket_file == NULL) {
    create_bucket_file();
    bucket_file = fopen(BUCKET_FILE, "rb+");
    if (bucket_file == NULL) {
      perror("Failed to reopen bucket file after creation");
      return NULL;
    }
  }

  struct bucket *buck =
      (struct bucket *)malloc(sizeof(struct bucket) * h->total_buckets);

  for (short i = 0; i < h->total_buckets; i++) {
    fseek(bucket_file, h->bucket_address[i], SEEK_SET);

    fread(&buck[i].local_depth, sizeof(buck[i].local_depth), 1, bucket_file);

    fread(&buck[i].total_elements, sizeof(buck[i].total_elements), 1,
          bucket_file);

    buck[i].list_start = create_node_bucket();
    struct node_bucket *current_node = NULL;
    struct node_bucket *current_next = buck[i].list_start;

    for (short j = 0; j < buck->total_elements; j++) {
      current_node = create_node_bucket();

      fread(&current_node->element.address,
            sizeof(current_node->element.address), 1, bucket_file);
      fread(&current_node->element.id, sizeof(current_node->element.id), 1,
            bucket_file);

      current_next->next = current_node;
      current_next = current_next->next;
    }
  }

  fclose(bucket_file);
  return buck;
}
int8_t hash_bucket(int id, short global_depth) {
  int p = pow(2, global_depth);
  p = id % (int)p;
  // printf("cesto num %d \n \n", p);
  return abs(p);
}
int8_t hash_bucket_local(int id, short local_depth) {
  int p = pow(2, local_depth);
  p = id % (int)p;
  // printf("cesto num %d \n \n", p);
  return abs(p);
}
int double_hash(struct hash_address_id *h) {
  if (h->global_depth >= 8)
    return 0;
  else {
    h->global_depth++;
    h->total_buckets *= 2;
    int q1 = (int)pow(2, h->global_depth - 1);
    int q2 = (int)pow(2, h->global_depth);
    long new_address[q2];
    int i = 0;
    while (i < q1) {
      new_address[i] = h->bucket_address[i];
      i++;
    }
    while (i < q2) {
      new_address[i] = h->bucket_address[i - q1];
      i++;
    }
    for (i = 0; i < q2; i++)
      h->bucket_address[i] = new_address[i];
    return 1;
  }
}
int64_t delete_bucket_file(short id) {
  struct hash_address_id *h = create_hash_address_id();
  read_hash_file(h);

  FILE *bucket_file = fopen(BUCKET_FILE, "rb+");
  long address = h->bucket_address[hash_bucket(id, h->global_depth)];
  struct bucket *b = create_bucket();
  fseek(bucket_file, address, SEEK_SET);

  fread(&b->local_depth, sizeof(b->local_depth), 1, bucket_file);
  fread(&b->total_elements, sizeof(b->total_elements), 1, bucket_file);

  struct node_bucket *free_ptr = b->list_start;

  int64_t task_location = -1;

  for (int i = 0; i < b->total_elements; i++) {
      free_ptr->next = create_node_bucket();
      free_ptr = free_ptr->next;
    if (free_ptr->element.id == id) {

      b->total_elements--;
      fread(&task_location, sizeof(int64_t), 1, bucket_file);
      fseek(bucket_file, 2, SEEK_CUR);

    } else {
      fread(&free_ptr->element.address, sizeof(free_ptr->element.address), 1,
            bucket_file);
      fread(&free_ptr->element.id, sizeof(free_ptr->element.id), 1,
            bucket_file);
    }
  }
  fclose(bucket_file);
  update_bucket_file(*b, address);

  return task_location;
}
int insert_bucket_file(struct address_id new) {
  struct hash_address_id *h = create_hash_address_id();

  read_hash_file(h);

  long address = h->bucket_address[hash_bucket(new.id, h->global_depth)];
  FILE *bucket_file = fopen(BUCKET_FILE, "rb+");

  // printf("onde esta meu bucket: %ld\n", address);
  fseek(bucket_file, address, SEEK_SET);
  // printf("onde estou no bucket: %ld\n", ftell(bucket_file));

  struct bucket *b = create_bucket();

  fread(&b->local_depth, sizeof(b->local_depth), 1, bucket_file);
  fread(&b->total_elements, sizeof(b->total_elements), 1, bucket_file);

  // printf("o total elements que esta sendo lido %hu\n",      (unsigned
  // short)b->total_elements);

  struct node_bucket *free_ptr = b->list_start;
  for (int i = 0; i < b->total_elements; i++) {
    if (i > 0) {
      free_ptr->next = create_node_bucket();
      free_ptr = free_ptr->next;
    }
    fread(&free_ptr->element.address, sizeof(free_ptr->element.address), 1,
          bucket_file);
    fread(&free_ptr->element.id, sizeof(free_ptr->element.id), 1, bucket_file);
  }

  if (b->total_elements < 24) {
    fseek(bucket_file, address, SEEK_SET);
    // printf("onde estou no bucket: %ld\n", ftell(bucket_file));
    fseek(bucket_file, 1, SEEK_CUR); // Adjust as necessary
    b->total_elements += 1;

    fwrite(&b->total_elements, sizeof(b->total_elements), 1, bucket_file);

    fseek(bucket_file, ELEMENT_SIZE * (b->total_elements - 1), SEEK_CUR);
    // printf("quanto estou pulando %d\n", ELEMENT_SIZE * (b->total_elements -
    // 1)); printf("onde estou colocando %ld", ftell(bucket_file));
    fwrite(&new.address, sizeof(new.address), 1, bucket_file);
    fwrite(&new.id, sizeof(new.id), 1, bucket_file);

    free(b);
    fclose(bucket_file);
    free(h);
    return 1;
  } else {
    if (b->local_depth >= h->global_depth) {
      double_hash(h);
      update_hash_file(*h);
      printf("dobrou %hu, %hu\n", h->global_depth, h->total_buckets);
    }
    fseek(bucket_file, address, SEEK_SET);

    struct bucket *new_b = create_bucket();

    int temp = b->local_depth;
    new_b->local_depth = temp + 1;

    fwrite(&new_b->local_depth, sizeof(new_b->local_depth), 1, bucket_file);
    fwrite(&(uint16_t){0}, sizeof(uint16_t), 1, bucket_file);

    struct bucket *old_b = create_bucket();

    long new_address;
    if (address == 0)
      new_address = pow(2, temp) * 256;
    else
      new_address = address + pow(2, temp) * 256;

    fseek(bucket_file, new_address, SEEK_SET);
    old_b->local_depth = h->global_depth;

    fwrite(&old_b->local_depth, sizeof(old_b->local_depth), 1, bucket_file);
    fwrite(&(uint16_t){0}, sizeof(uint16_t), 1, bucket_file);

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

    update_hash_file(*h);
  }

  struct node_bucket *tmp = b->list_start;
  int total_elements = b->total_elements;
  for (int i = 0; i < total_elements; i++) {
    struct node_bucket *next = tmp->next;
    insert_bucket_file(tmp->element);
    tmp = next;
  }
  insert_bucket_file(new);
  free(tmp);
  free(b);
  return 0;
}
