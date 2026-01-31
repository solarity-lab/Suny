#ifndef SEVAL_H
#define SEVAL_H

#include "sobj.h"
#include "sframe.h"
#include "sstr.h"
#include "sbool.h"
#include "score.h"
#include "slist.h"
#include "serror.h"

struct Slist;
struct Sframe;
struct Sobj;

SUNY_API struct Sobj *
Seval_add
(struct Sobj *obj1, struct Sobj *obj2);

SUNY_API struct Sobj *
Seval_mod
(struct Sobj *obj1, struct Sobj *obj2);

SUNY_API struct Sobj *
Seval_sub
(struct Sobj *obj1, struct Sobj *obj2);

SUNY_API struct Sobj *
Seval_mul
(struct Sobj *obj1, struct Sobj *obj2);

SUNY_API struct Sobj *
Seval_div
(struct Sobj *obj1, struct Sobj *obj2);

SUNY_API struct Sobj *
Seval_bigger
(struct Sobj *obj1, struct Sobj *obj2);

SUNY_API struct Sobj *
Seval_smaller
(struct Sobj *obj1, struct Sobj *obj2);

SUNY_API struct Sobj *
Seval_equal
(struct Sobj *obj1, struct Sobj *obj2);

SUNY_API struct Sobj *
Seval_not_equal
(struct Sobj *obj1, struct Sobj *obj2);

SUNY_API struct Sobj *
Seval_bigger_and_equal
(struct Sobj *obj1, struct Sobj *obj2);

SUNY_API struct Sobj *
Seval_smaller_and_equal
(struct Sobj *obj1, struct Sobj *obj2);

int Slist_cmp(struct Slist *list1, struct Slist *list2);

SUNY_API int 
Seval_evaluate_mod
(struct Sframe *frame);

SUNY_API int
Seval_evaluate_add
(struct Sframe *frame);

SUNY_API int
Seval_evaluate_sub
(struct Sframe *frame);

SUNY_API int
Seval_evaluate_mul
(struct Sframe *frame);

SUNY_API int
Seval_evaluate_div
(struct Sframe *frame);

SUNY_API int
Seval_evaluate_bigger
(struct Sframe *frame);

SUNY_API int
Seval_evaluate_smaller
(struct Sframe *frame);

SUNY_API int
Seval_evaluate_equal
(struct Sframe *frame);

SUNY_API int
Seval_evaluate_not_equal
(struct Sframe *frame);

SUNY_API int
Seval_evaluate_bigger_and_equal
(struct Sframe *frame);

SUNY_API int
Seval_evaluate_smaller_and_equal
(struct Sframe *frame);

#endif
