#ifndef SLOGIC_H
#define SLOGIC_H

#include "sobj.h"

#define Lis(a, b) ((a) == (b))
#define Lor(a, b) ((a) || (b))
#define Land(a, b) ((a) && (b))
#define Lnot(a) (!(a))

int Slogic_AND(struct Sobj* a, struct Sobj* b);
int Slogic_OR(struct Sobj* a, struct Sobj* b);
int Slogic_IS(struct Sobj* a, struct Sobj* b);
int Slogic_NOT(struct Sobj* obj);

#endif // SLOGIC_H