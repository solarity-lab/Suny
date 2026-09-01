#include "srange.h"
#include "smem.h"

int Srange_free(struct Srange* range) {
    Smem_Free(range->range);
    Smem_Free(range);
    return 0;
}

SUNY_API struct Srange* Srange_new(void) {
    struct Srange* range = Smem_Malloc(sizeof(struct Srange));
    range->range = Smem_Malloc(sizeof(int) * 1024);
    range->size = 0;
    range->capacity = 1024;

    range->start = 0;
    range->end = 0;
    return range;
}

SUNY_API struct Srange* Srange_creat(int a, int b) {
    struct Srange* range = Srange_new();
    for (int i = a; i <= b; i++) {
        if (range->size >= range->capacity) {
            range->capacity *= 2;
            range->range = Smem_Realloc(range->range, sizeof(int) * range->capacity);
        }
        range->range[range->size++] = i;
    }

    range->start = a;
    range->end = b;
    return range;
}

int Srange_get(struct Srange* range, int index) {
    if (index >= range->size || index < 0) {
        return 0;
    }
    return range->range[index];
}

SUNY_API struct Sobj* Sobj_make_range(int a, int b) {
    struct Sobj* sobj = Sobj_make_type(RANGE_OBJ);
    sobj->f_type = Stype_new();
    tget_range(sobj) = Srange_creat(a, b);
    return sobj;
}