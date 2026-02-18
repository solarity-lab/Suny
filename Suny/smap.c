#include "smap.h"
#include "smem.h"

struct Shashmap* Shashmap_new(void) {
    struct Shashmap* map = Smem_Malloc(sizeof(struct Shashmap));

    map->capacity = MAX_HASH_CAPACITY;
    map->count = 0;
    map->buckets = Smem_Calloc(map->capacity, sizeof(struct Shash_entry*));

    return map;
}

struct Shash_entry* HashEntrySet(struct Sobj* key, struct Sobj* value) {
    struct Shash_entry* entry = Smem_Malloc(sizeof(struct Shash_entry));

    entry->hash = Shash(key);
    entry->key = key;
    entry->value = value;
    entry->next = NULL;

    return entry;
}

int Shashmap_store_pair(struct Sframe* frame, struct Shashmap* map, struct Sobj* key, struct Sobj* value) {
    hash_t hash = Shash(key);
    int index = GetIndex(hash, map);

    struct Shash_entry* entry = map->buckets[index];

    while (entry) {
        if (entry->hash == hash && Scompare_representative(entry->key, key, CMP_EQ)) {
            struct Sobj* old = entry->value;

            _SUNYINCREF(value);

            if (old) {
                _SUNYDECREF(old);
                MOVETOGC(old, frame->gc_pool);
            }

            entry->value = value;
            return 1;
        }
        entry = entry->next;
    }

    struct Shash_entry* new_entry = HashEntrySet(key, value);

    _SUNYINCREF(key);
    _SUNYINCREF(value);

    new_entry->next = map->buckets[index];
    map->buckets[index] = new_entry;

    map->count++;

    return 1;
}

struct Sobj* Shashmap_get(struct Shashmap* map, struct Sobj* key) {
    hash_t hash = Shash(key);
    int index = GetIndex(hash, map);

    struct Shash_entry* entry = map->buckets[index];

    while (entry) {
        if (entry->hash == hash &&
            Scompare_representative(entry->key, key, CMP_EQ)) {
            return entry->value;
        }
        entry = entry->next;
    }

    return NULL;
}

struct Sobj* Sobj_make_hashmap(struct Sframe* frame, struct Slist* l1, struct Slist* l2) {
    if (!l1 || !l2 || l1->count != l2->count) return NULL;

    struct Shashmap* map = Shashmap_new();

    for (int i = 0; i < l1->count; i++) {
        Shashmap_store_pair(frame, map, l1->array[i], l2->array[i]);
    }

    struct Sobj* obj = Sobj_new();
    obj->type = MAP_OBJ;
    obj->f_type = Stype_new();
    obj->f_type->f_hashmap = map;

    return obj;
}

int Shashmap_free(struct Sframe* frame, struct Shashmap* map) {
    if (!map) return 0;

    for (int i = 0; i < map->capacity; i++) {
        struct Shash_entry* entry = map->buckets[i];
        while (entry) {
            struct Shash_entry* next = entry->next;
            
            _SUNYDECREF(entry->key);
            _SUNYDECREF(entry->value);
            MOVETOGC(entry->key, frame->gc_pool);
            MOVETOGC(entry->value, frame->gc_pool);
            
            Smem_Free(entry);
            entry = next;
        }
    }
    
    Smem_Free(map->buckets);
    Smem_Free(map);
    return 0;
}