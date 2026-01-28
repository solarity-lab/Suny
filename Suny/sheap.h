#ifndef SHEAP_H
#define SHEAP_H

#include "sobj.h"
#include "max.h"

#define Sframe_initialize_heap(frame) ((frame)->f_heap = Sheap_new())

struct Sheap {
    struct Sobj** f_heap;
    int f_heap_size;
    int f_heap_index; 
};

struct Sheap* Sheap_new(void);

int Sheap_free(struct Sheap *heap);

int Sheap_store(struct Sheap *heap, struct Sobj *obj);

struct Sobj* Sheap_get(struct Sheap *heap, int address);

#endif // SHEAP_H