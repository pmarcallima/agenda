#ifndef BUCKET_H
#define BUCKET_H
#define QNT_MAX_ELE 32767
#define MAX_DEPTH 127
#define INITIAL_BUCKET_SIZE 8
#define BUCKET_BYTE_SIZE 32767
#define FLEX_HASH_FILE "hash_address_id.txt"
#define BUCKET_FILE "bucket.txt"

#include <stdint.h>

struct addres_id {
  int16_t id;
  intptr_t address;
};

struct node_bucket {
  struct addres_id element;
  struct node_bucket *next;
};

struct bucket {

  int8_t local_depth;
  int16_t bytes_per_element;
  int16_t total_elements;
  struct node_bucket **list_start;
};
struct hash_addres_id {
  int8_t global_depth;
  int16_t element_amount;
  int8_t total_buckets;
  int64_t bucket_address[MAX_DEPTH];
};

#endif // !BUCKET_H
