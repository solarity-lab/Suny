#ifndef SCOMPARE_H
#define SCOMPARE_H

#include "score.h"
#include "sobj.h"

#define CMP_GT   1
#define CMP_LT   2
#define CMP_EQ   3
#define CMP_NEQ  4
#define CMP_GE   5
#define CMP_LE   6

#define CMP_FALSE 0
#define CMP_TRUE  1

#define Scompare_not_equal(o1, o2) !Scompare_equal(o1, o2)
#define Scompare_bigger_equal(o1, o2) Scompare_bigger(o1, o2) || Scompare_equal(o1, o2)
#define Scompare_smaller_equal(o1, o2) Scompare_smaller(o1, o2) || Scompare_equal(o1, o2)

int Scompare(struct Sobj* a, struct Sobj* b, int op);
int Scompare_bigger(struct Sobj* obj1, struct Sobj* obj2);
int Scompare_smaller(struct Sobj* obj1, struct Sobj* obj2);
int Scompare_equal(struct Sobj* obj1, struct Sobj* obj2);

#endif // SCOMPARE_H