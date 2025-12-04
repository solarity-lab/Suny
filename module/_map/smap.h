#ifndef SMAP_H
#define SMAP_H

#include "C:\Suny\src\Suny.h"

struct Smap {
    struct Sobj** keys;
    struct Sobj** values;
    int capacity;
    int count;
};

struct Sobj* Sobj_set_map(struct Sframe* frame);

struct Sobj* Sobj_get_map_key(struct Sobj* map, struct Sobj* key);
struct Sobj* Sobj_print_map(struct Sobj* map);
struct Sobj* Sobj_map_update(struct Sobj* map1, struct Sobj* map2);
int Sobj_set_map_key(struct Sobj* map, struct Sobj* key, struct Sobj* value);

#endif // SMAP_H