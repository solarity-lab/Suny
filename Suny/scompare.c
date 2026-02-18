#include "scompare.h"
#include "sdebug.h"
#include "sobj.h"

static int call_meta(struct Sobj* a, struct Sobj* b, struct Sobj* (*fn)(struct Sobj*, struct Sobj*)) {
    if (!fn) return CMP_FALSE;

    struct Sobj* r = fn(a, b);
    if (!r || !r->value) return CMP_FALSE;

    return ValueOf(r);
}

static int compare_raw(struct Sobj* a, struct Sobj* b, int op) {
    int x = ValueOf(a);
    int y = ValueOf(b);

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
    return cmp_by_result(Sstr_cmp(tget_str(a), tget_str(b)), op);
}

static int compare_list(struct Sobj* a, struct Sobj* b, int op) {
    return cmp_by_result(Slist_cmp(tget_list(a), tget_list(b)), op);
}

static int compare_user_data(struct Sobj* a, struct Sobj* b, int op) {
    struct Smeta* m1 = MetaOf(a);
    struct Smeta* m2 = MetaOf(b);

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
    if (!a || !b || TypeOf(a) == NULL_OBJ || TypeOf(b) == NULL_OBJ)
        __ERROR("attempt to compare null value");

    if (TypeOf(a) == STRING_OBJ && TypeOf(b) == STRING_OBJ) return compare_string(a, b, op);
    if (TypeOf(a) == LIST_OBJ && TypeOf(b) == LIST_OBJ) return compare_list(a, b, op);
    if (TypeOf(a) == USER_DATA_OBJ || TypeOf(b) == USER_DATA_OBJ) return compare_user_data(a, b, op);

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

int Scompare_representative(struct Sobj* a, struct Sobj* b, int op) {
    if (TypeOf(a) == TRUE_OBJ || TypeOf(a) == FALSE_OBJ) {
        switch (op) {
            case CMP_EQ:
                {
                    if (TypeOf(a) != TypeOf(b))
                        return 0;
                    else
                        return 1;
                }
            case CMP_NEQ: 
                {
                    if (TypeOf(a) != TypeOf(b))
                        return 1;
                    else
                        return 0;
                }
        }
    } else if (TypeOf(b) == TRUE_OBJ || TypeOf(b) == FALSE_OBJ) {
        switch (op) {
            case CMP_EQ:
                {
                    if (TypeOf(a) != TypeOf(b))
                        return 0;
                    else
                        return 1;
                }
            case CMP_NEQ:
                {
                    if (TypeOf(a) != TypeOf(b))
                        return 1;
                    else
                        return 0;
                }
        }
    }

    return Scompare(a, b, op);
}
