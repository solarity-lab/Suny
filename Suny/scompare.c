#include "scompare.h"
#include "sdebug.h"

static int call_meta(struct Sobj* a, struct Sobj* b, struct Sobj* (*fn)(struct Sobj*, struct Sobj*)) {
    if (!fn) return CMP_FALSE;

    struct Sobj* r = fn(a, b);
    if (!r || !r->value) return CMP_FALSE;

    return r->value->value;
}

static int compare_raw(struct Sobj* a, struct Sobj* b, int op) {
    int x = a->value->value;
    int y = b->value->value;

    if (op == CMP_EQ)  return x == y;
    if (op == CMP_LT)  return x < y;
    if (op == CMP_GT)  return x > y;
    if (op == CMP_NEQ) return x != y;
    if (op == CMP_GE)  return x >= y;
    if (op == CMP_LE)  return x <= y;

    return CMP_FALSE;
}

static int cmp_by_result(int cmp, int op) {
    if (op == CMP_EQ)  return cmp == 0;
    if (op == CMP_LT)  return cmp < 0;
    if (op == CMP_GT)  return cmp > 0;
    if (op == CMP_NEQ) return cmp != 0;
    if (op == CMP_GE)  return cmp >= 0;
    if (op == CMP_LE)  return cmp <= 0;

    return CMP_FALSE;
}

static int compare_string(struct Sobj* a, struct Sobj* b, int op) {
    return cmp_by_result(Sstr_cmp(a->f_type->f_str, b->f_type->f_str), op);
}

static int compare_list(struct Sobj* a, struct Sobj* b, int op) {
    return cmp_by_result(Slist_cmp(a->f_type->f_list, b->f_type->f_list), op);
}

static int compare_user_data(struct Sobj* a, struct Sobj* b, int op) {
    struct Smeta* m1 = a->meta;
    struct Smeta* m2 = b->meta;

    if (op == CMP_EQ) {
        if (m1 && m1->mm_eq) return call_meta(a, b, m1->mm_eq);
        if (m2 && m2->mm_eq) return call_meta(a, b, m2->mm_eq);
    }

    else if (op == CMP_LT && m1 && m1->mm_lt) return call_meta(a, b, m1->mm_lt);

    else if (op == CMP_GT && m1 && m1->mm_gt) return call_meta(a, b, m1->mm_gt);

    else if (op == CMP_NEQ && m1 && m1->mm_ne) return call_meta(a, b, m1->mm_ne);

    else if (op == CMP_GE && m1 && m1->mm_ge) return call_meta(a, b, m1->mm_ge);

    else if (op == CMP_LE && m1 && m1->mm_le) return call_meta(a, b, m1->mm_le);

    return CMP_FALSE;
}

int Scompare(struct Sobj* a, struct Sobj* b, int op) {
    if (!a || !b || a->type == NULL_OBJ || b->type == NULL_OBJ)
        __ERROR("attempt to compare null value");

    if (a->type == STRING_OBJ && b->type == STRING_OBJ) return compare_string(a, b, op);

    if (a->type == LIST_OBJ && b->type == LIST_OBJ) return compare_list(a, b, op);

    if (a->type == USER_DATA_OBJ || b->type == USER_DATA_OBJ) return compare_user_data(a, b, op);

    return compare_raw(a, b, op);
}

int Scompare_bigger(struct Sobj* a, struct Sobj* b) {
    return Scompare(a, b, CMP_GT);
}

int Scompare_smaller(struct Sobj* a, struct Sobj* b) {
    return Scompare(a, b, CMP_LT);
}

int Scompare_equal(struct Sobj* a, struct Sobj* b) {
    return Scompare(a, b, CMP_EQ);
}
