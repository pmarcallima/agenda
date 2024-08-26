#include "byte.h"
#include <bucket.h>
#include <stdio.h>
#include <stdlib.h>

struct hash_addres_id *create_hash_addres_id(void) {
  struct hash_addres_id *tmp =
      (struct hash_addres_id *)malloc(sizeof(struct hash_addres_id));
  tmp->global_depth = -1;
  tmp->element_amount = -1;
  for (int i = 0; i < MAX_DEPTH; i++) {
    tmp->bucket_address[i] = -1;
  }
  return tmp;
}
struct node_bucket *create_node_bucket(void)
{
  struct node_bucket * tmp = (struct node_bucket *)malloc(sizeof(struct node_bucket));
  tmp->next = NULL;
  tmp->element.address = -1;
  tmp->element.id = -1;
  return tmp;
}
void create_hash_file(void) {
  FILE *hash_file = fopen(FLEX_HASH_FILE, "wb+");
  fwrite(0, 1, 1, hash_file);
  fwrite(0, 1, 1, hash_file);
  char tmp = -1;
  fwrite(&tmp, 1, 1, hash_file);
  fclose(hash_file);
}
void read_hash_file(struct hash_addres_id *h) {
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
  for (short i = 0; i < h->total_buckets; i++) {
    fread(&h->bucket_address[i], sizeof(int64_t), 1, hash_file);
    fseek(hash_file, 32766, SEEK_CUR);
  }
}
struct bucket* read_all_bucket_file(struct hash_addres_id *h, struct bucket *b) {
  FILE *bucket_file = fopen(BUCKET_FILE, "rb+");
  struct node_bucket *tmp = create_node_bucket();
  struct node_bucket *free_ptr = NULL;
  struct bucket *buck = (struct bucket*)malloc(sizeof(struct bucket)*h->total_buckets);
  

  
  for(short i = 0; i< h->total_buckets; i++)
  {
    fseek(bucket_file, h->bucket_address[i], SEEK_SET);
    fread(&b[i].local_depth, 1, 1, bucket_file);
    fread(&b[i].total_elements, 2, 1, bucket_file);
    fread(&b[i].bytes_per_element, 2, 1, bucket_file);
    b[i].list_start = &tmp;
    for(short j = 0; j< b[i].total_elements; j++)
    {
    fread(&tmp->element.address, 8, 1, bucket_file);
    fread(&tmp->element.id, 2, 1, bucket_file );
    tmp->next = create_node_bucket();
    tmp = tmp->next;
    }
    


    
  }

}

void insert_hash_file(struct hash_addres_id h) {}
