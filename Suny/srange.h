#ifndef SRANGE_H
#define SRANGE_H

#include "sobj.h"
#include <math.h>

struct Srange {
    int start, end, size;
};

int Srange_free(struct Srange* range);

SUNY_API struct Srange* Srange_new(void);
SUNY_API struct Srange* Srange_creat(int a, int b);
int Srange_get(struct Srange* range, int index);

SUNY_API struct Sobj* Sobj_make_range(int a, int b);

#endif // SRANGE_H