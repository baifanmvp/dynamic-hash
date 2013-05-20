#ifndef DYNHASH_H_
#define DYNHASH_H_
#include "hashfunction.h"
#include "dynarray.h"
typedef unsigned char uchar;
typedef uchar* (*dynhash_get_key)(const uchar *);
typedef void (*dynhash_free_key)(void *);

typedef unsigned long (*dynhash_function)(unsigned char *key);
typedef struct st_dynhnode
{
    void* data;
    size_t next;
}DYNHNODE;

typedef struct st_dynhash
{
    size_t hmod;   // hashval % hmod
    ulong hlen;    // hash map size
    uint flags;
    DYNARRAY* array;				/* Place for hash_keys */
    dynhash_get_key getkey;
    dynhash_free_key freekey;
    dynhash_function fnhash;
} DYNHASH;

#define HNODE_INVALID_IDX ((size_t)(-1))


DYNHASH* dynhash_new(dynhash_get_key getkey, dynhash_free_key freekey, dynhash_function fnhash);

int dynhash_delete(DYNHASH* phash);

unsigned long dynhash_hash_pos(DYNHASH* phash, void* key);

DYNHNODE* dynhash_search(DYNHASH* phash, void* data);

int dynhash_insert (DYNHASH* phash, void* data);

#endif
