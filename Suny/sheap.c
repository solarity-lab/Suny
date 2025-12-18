#include "sheap.h"
#include "smem.h"

struct Sheap* Sheap_new(void) {
    struct Sheap* heap = Smem_Malloc(sizeof(struct Sheap));

    heap->f_heap = Smem_Calloc(1024, sizeof(struct Sobj*));
    heap->f_heap_size = 1024;
    heap->f_heap_index = 0;

    return heap;
}

int Sheap_free(struct Sheap *heap) {
    Smem_Free(heap->f_heap);
    Smem_Free(heap);
    return 0;
}

int Sheap_store(struct Sheap *heap, struct Sobj *obj) {
    if (heap->f_heap_index >= heap->f_heap_size) {
        heap->f_heap_size *= 2;
        heap->f_heap = Smem_Realloc(heap->f_heap, heap->f_heap_size * sizeof(struct Sobj*));
    }

    SUNYINCREF(obj->f_value);

    heap->f_heap[heap->f_heap_index++] = obj;
    return 0;
}

struct Sobj* Sheap_get(struct Sheap *heap, int address) {
    for (int i = 0; i < heap->f_heap_index; i++) {
        if (heap->f_heap[i]->address == address) {
            return heap->f_heap[i];
        }
    }
    return NULL;
}