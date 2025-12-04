#include "smap.h"

struct Smap* Smap_new(void) {
    struct Smap* map = Smem_Malloc(sizeof(struct Smap));
    map->capacity = 4;
    map->count = 0;
    map->keys = Smem_Calloc(map->capacity, sizeof(struct Sobj*));
    map->values = Smem_Calloc(map->capacity, sizeof(struct Sobj*));
    return map;
}

static void Smap_ensure_capacity(struct Smap* map) {
    if (map->count >= map->capacity) {
        int new_capacity = map->capacity * 2;
        map->keys = Smem_Realloc(map->keys, sizeof(struct Sobj*) * new_capacity);
        map->values = Smem_Realloc(map->values, sizeof(struct Sobj*) * new_capacity);
        map->capacity = new_capacity;
    }
}

void Smap_push(struct Smap* map, struct Sobj* key, struct Sobj* value) {
    Smap_ensure_capacity(map);

    _SUNYINCREF(key);
    _SUNYINCREF(value);

    map->keys[map->count] = key;
    map->values[map->count] = value;
    map->count++;
}

void Smap_set(struct Smap* map, struct Slist* lmap) {
    int count = lmap->count / 2;
    
    if (lmap->count % 2 != 0) {
        printf("Error: lmap must contain even number of elements (key-value pairs)\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        struct Sobj* key = lmap->array[i * 2];
        struct Sobj* value = lmap->array[i * 2 + 1];
        Smap_push(map, key, value);
    }
}

void Smap_pop(struct Smap* map) {
    struct Sobj* key = map->keys[map->count - 1];
    struct Sobj* value = map->values[map->count - 1];

    _SUNYDECREF(key);
    _SUNYDECREF(value);

    map->count--;
}

struct Sobj* Sobj_set_map(struct Sframe* frame) {
    struct Sobj* list = Sframe_pop(frame);
    struct Slist* lmap = list->f_type->f_list;
    
    struct Smap* map = Smap_new();
    
    Smap_set(map, lmap);
    
    struct Sobj* obj = Sobj_make_userdata(map);
    obj->meta = Smeta_new();
    
    obj->meta->mm_tostring = Sobj_print_map;
    obj->meta->mm_index = Sobj_get_map_key;
    obj->meta->mm_store_item = Sobj_set_map_key;

    return obj;
}

struct Sobj* Sobj_get_map_key(struct Sobj* map, struct Sobj* key) {
    struct Smap* mmap = get_userdata(map);
    for (int i = 0; i < mmap->count; i++) {
        if (Scompare_equal(mmap->keys[i], key)) {
            return mmap->values[i];
        }
    }    

    return null_obj;
}

struct Sobj* Sobj_map_update(struct Sobj* map1, struct Sobj* map2) {
    struct Smap* mmap1 = get_userdata(map1);
    struct Smap* mmap2 = get_userdata(map2);
    for (int i = 0; i < mmap2->count; i++) {
        Smap_push(mmap1, mmap2->keys[i], mmap2->values[i]);
    }
    return map1;
}

int Sobj_set_map_key(struct Sobj* map, struct Sobj* key, struct Sobj* value) {
    struct Smap* mmap = get_userdata(map);

    for (int i = 0; i < mmap->count; i++) {
        if (Scompare_equal(mmap->keys[i], key)) {
            _SUNYDECREF(mmap->values[i]);

            if (mmap->values[i]->gc->ref_count < 1) {
                Sobj_free(mmap->values[i]);
            }

            mmap->values[i] = value;
            return 0;
        }
    }

    _SUNYINCREF(value);
    Smap_push(mmap, key, value);
    return 0;
}

struct Sobj* Sobj_print_map(struct Sobj* map) {
    struct Smap* mmap = get_userdata(map);
    printf("{");
    for (int i = 0; i < mmap->count; i++) {
        printf("%s: %s", Sio_sprintf(mmap->keys[i]), Sio_sprintf(mmap->values[i]));

        if (i < mmap->count - 1) {
            printf(", ");
        }
    }
    printf("}");
    return null_obj;
}

SUNY_API struct Sframe* Smain(struct Sframe* frame, struct Scompiler* compiler) {
    Sinitialize_c_api_func(frame, compiler, 1, "map", 1, Sobj_set_map);
    return frame;    
}