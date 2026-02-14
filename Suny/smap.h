#ifndef SMAP_H
#define SMAP_H

#include "sobj.h"
#include "slist.h"
#include "stype.h"      

struct Smap* Smap_new(void);

int Smap_add_key(struct Smap* map, struct Sobj* key);
int Smap_add_value(struct Smap* map, struct Sobj* value);
int Smap_add_pair(struct Smap* map, struct Sobj* key, struct Sobj* value);

struct Sobj* Smap_get_value(struct Smap* map, struct Sobj* key);

struct Sobj* Sobj_make_map(struct Smap* map);

int Smap_free(struct Smap* map);

#endif // SMAP_H
