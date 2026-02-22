#include "sgc.h"
#include "sdebug.h"
#include "smem.h"

struct Garbage_pool* Sgc_new_pool(void) {
    SDEBUG("[sgc.c] struct Garbage_pool* Sgc_new_pool(void) (building...)\n");

    struct Garbage_pool* pool = Smem_Malloc(sizeof(struct Garbage_pool));
    pool->pool_size = 50000;
    pool->pool_index = 0;

    pool->garbage_pool = Smem_Malloc(sizeof(struct Sobj*) * pool->pool_size);
    
    SDEBUG("[sgc.c] struct Garbage_pool* Sgc_new_pool(void) (done)\n");
    return pool;
}

struct Garbage_pool* Sgc_push_garbage_obj(struct Garbage_pool* pool, struct Sobj* obj) {
    if (pool->pool_index >= pool->pool_size) {
        pool->pool_size *= 2;
        pool->garbage_pool = Smem_Realloc(pool->garbage_pool, pool->pool_size * sizeof(struct Sobj*));
    }

    pool->garbage_pool[pool->pool_index++] = obj;
    return pool;
}

void Sgc_collect(struct Garbage_pool* pool) {
    for (int i = 0; i < pool->pool_index; i++) {
        struct Sobj* obj = pool->garbage_pool[i];

        if (!obj) {
            printf("Null object found in garbage pool\n");
            SUNY_BREAK_POINT;
        }

        if (obj->ref < 1) {
            Sobj_free(obj);
        }

        pool->garbage_pool[i] = NULL;
    }

    pool->pool_index = 0;

    return;
}

int MOVETOGC(struct Sobj* obj, struct Garbage_pool* pool) {
    if (!pool) {
        __ERROR("Error: pool is null\n");
    }

    if (obj->ref < 1) {
        Sgc_push_garbage_obj(pool, obj);
    }
    return 0;
}

int Sgc_free_pool(struct Garbage_pool* pool) {
#ifdef DEBUG
    printf("[sgc.c] int Sgc_free_pool(struct Garbage_pool* pool) (building...)\n");
#endif

    Smem_Free(pool->garbage_pool);
    Smem_Free(pool);

#ifdef DEBUG
    printf("[sgc.c] int Sgc_free_pool(struct Garbage_pool* pool) (done)\n");
#endif
    return 0;
}