#ifndef SGC_H
#define SGC_H

#include "score.h"
#include "sobj.h"

struct Sframe;

#define Sframe_initialize_gc(frame) ((frame)->gc_pool = Sgc_new_pool())

struct Garbage_pool {
    struct Sobj** garbage_pool;
    
    int pool_size;
    int pool_index;

    int is_activate;
};

#define _SUNYINCREF(obj) ((obj)->ref++)
#define _SUNYDECREF(obj) ((obj)->ref--)

#define POOL_SIZE_LIMIT 500 /*for the fastest performance set it to 500*/

struct Garbage_pool* Sgc_new_pool(void);
int Sgc_free_pool(struct Garbage_pool* pool);
struct Garbage_pool* Sgc_push_garbage_obj(struct Garbage_pool* pool, struct Sobj* obj);

void Sgc_collect(struct Garbage_pool* pool);

int MOVETOGC(struct Sobj* obj, struct Garbage_pool* pool);

int Sgc_activate(struct Sframe *frame);
int Sgc_deactivate(struct Sframe *frame);

#endif // SGC_H