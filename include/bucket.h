#ifndef BUCKET_H
#define BUCKET_H
#define QNT_MAX_ELE 256
#define MAX_DEPTH 8 
#define INITIAL_BUCKET_QNT 8
#define BUCKET_BYTE_SIZE 13
#define FLEX_HASH_FILE "hash_address_id.txt"
#define BUCKET_FILE "bucket.txt"
#define ELEMENT_SIZE 10

#include <stdint.h>

struct address_id {
  int16_t id;
  intptr_t address;
};

struct node_bucket {
  struct address_id element;
  struct node_bucket *next;
};

struct bucket {

  int8_t local_depth;
  int16_t total_elements;
  struct node_bucket **list_start;
};
struct hash_address_id {
  int8_t global_depth;
  int16_t element_amount;
  int8_t total_buckets;
  int64_t bucket_address[QNT_MAX_ELE];
};

#endif // !BUCKET_H
