#include "smap.h"
#include "scompare.h"
#include "smem.h"
#include "sdebug.h"
#include "sobj.h"
#include "stype.h"

struct Sobj* Sobj_make_map(struct Smap* map) {
    struct Sobj* obj = Sobj_new();

    obj->type = MAP_OBJ;
    obj->f_type = Stype_new();
    obj->f_type->f_map = map;

    return obj;
}

struct Smap* Smap_new(void) {
    struct Smap* map = (struct Smap*) Smem_Malloc(sizeof(struct Smap));
    map->keys = NULL;
    map->values = NULL;
    map->size = 0;

    return map;
}

int Smap_add_key(struct Smap *map, struct Sobj* key) {
    if (!map->keys)
        __ERROR("null keys");

    _SUNYINCREF(key);
    Slist_add(map->keys, key);
    return 0;
}

int Smap_add_value(struct Smap* map, struct Sobj* value) {
    if (!map->values)
        __ERROR("null values");

    _SUNYINCREF(value);
    Slist_add(map->values, value);
    return 0;
}

int Smap_add_pair(struct Smap* map, struct Sobj* key, struct Sobj* value) {
    Smap_add_key(map, key);
    Smap_add_value(map, value);
    map->size++;
    return 0;
}

struct Sobj* Smap_get_value(struct Smap* map, struct Sobj* key) {
    if (!map->values || !map->keys)
        __ERROR("null values and keys");

    for (int i = 0; i < map->size; i++) {
        struct Sobj* mkey = map->keys->array[i];
        if (Scompare_representative(mkey, key, CMP_EQ)) {
            struct Sobj* mvalue = map->values->array[i];
            return mvalue;
        }
    }

    return NULL;
}
