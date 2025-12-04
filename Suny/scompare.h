#ifndef SCOMPARE_H
#define SCOMPARE_H

#include "score.h"
#include "sobj.h"

#define Scompare_not_equal(o1, o2) !Scompare_equal(o1, o2)
#define Scompare_bigger_equal(o1, o2) Scompare_bigger(o1, o2) || Scompare_equal(o1, o2)
#define Scompare_smaller_equal(o1, o2) Scompare_smaller(o1, o2) || Scompare_equal(o1, o2)

int Scompare_bigger(struct Sobj* obj1, struct Sobj* obj2);
int Scompare_smaller(struct Sobj* obj1, struct Sobj* obj2);
int Scompare_equal(struct Sobj* obj1, struct Sobj* obj2);

#endif // SCOMPARE_H