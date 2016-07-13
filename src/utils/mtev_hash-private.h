#ifndef MTEV_HASH_TABLE_PRIVATE_H
#define MTEV_HASH_TABLE_PRIVATE_H

#include <ck_hs.h>

struct mtev_hash_table
{
  union {
    ck_hs_t hs;
    /**
     * This is evil.  In order to maintain ABI compat 
     * we are sneaking lock info into a pointer
     * in the leftover space for cache alignment
     * 
     * A ck_hs_t is ~48 bytes but since it has
     * always been declared up to a cache line
     * there is trailing space we can sneak a 
     * pointer into
     */
    struct {
      char pad[sizeof(ck_hs_t)];
      void *locks;
    } locks;
  } u CK_CC_CACHELINE;
};

/* mdb support relies on this being exposed */
typedef struct ck_key {
  u_int32_t len;
  char label[1];
} ck_key_t;

typedef struct ck_hash_attr {
  void *data;
  void *key_ptr;
  ck_key_t key;
} ck_hash_attr_t;

CK_CC_CONTAINER(ck_key_t, struct ck_hash_attr, key,
                index_attribute_container)


#endif
