#ifndef SMAP_H
#define SMAP_H

#include "stype.h"
#include "sobj.h"

#define MAX_HASH_CAPACITY 1024

#define GetIndex(hash, map) ((hash) & ((map)->capacity - 1))

struct Shash_entry* HashEntrySet(struct Sobj* key, struct Sobj* value);

struct Shashmap* Shashmap_new(void);

struct Sobj* Shashmap_get(struct Shashmap* map, struct Sobj* key);

int Shashmap_store_pair(struct Sframe* frame, struct Shashmap* map, struct Sobj* key, struct Sobj* value);

struct Sobj* Sobj_make_hashmap(struct Sframe* frame, struct Slist* l1, struct Slist* l2);

int Shashmap_free(struct Sframe* frame, struct Shashmap* map);

#endif // SMAP_H