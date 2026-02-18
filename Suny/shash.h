#ifndef SHASH_H
#define SHASH_H

#include <stdint.h>
#include "sobj.h"

typedef uint64_t hash_t;

#define TRUE_HASH   0x9e3779b97f4a7c15ULL
#define FALSE_HASH  0xc2b2ae3d27d4eb4fULL

#define NULL_HASH 0ULL
#define ERROR_HASH 9999ULL
hash_t Shash(struct Sobj* obj);

hash_t Shash_number(struct Sobj* obj);
hash_t Shash_string(struct Sobj* obj);
hash_t Shash_true(struct Sobj* obj);
hash_t Shash_false(struct Sobj* obj);
hash_t Shash_class(struct Sobj* obj);
hash_t Shash_function(struct Sobj* obj);

#endif // HASH_H