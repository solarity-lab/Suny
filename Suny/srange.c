#include "srange.h"
#include "smem.h"
#include "stool.h"

int Srange_free(struct Srange* range) {
    Smem_Free(range);
    return 0;
}

SUNY_API struct Srange* Srange_new(void) {
    struct Srange* range = Smem_Malloc(sizeof(struct Srange));

    range->size = 0;
    range->start = 0;
    range->end = 0;
    
    return range;
}

SUNY_API struct Srange* Srange_creat(int a, int b) {
    struct Srange* range = Srange_new();

    range->start = a;
    range->end = b;
    range->size = (int) fabs(b - a);

    return range;
}

int Srange_get(struct Srange* range, int index) {
    int size = range->size;

    if (index < 0 || index >= size) return 0;

    if (range->start <= range->end) return range->start + index;

    return range->start - index;
}

SUNY_API struct Sobj* Sobj_make_range(int a, int b) {
    struct Sobj* sobj = Sobj_make_type(RANGE_OBJ);
    sobj->f_type = Stype_new();
    tget_range(sobj) = Srange_creat(a, b);
    return sobj;
}