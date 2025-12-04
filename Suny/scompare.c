#include "scompare.h"

int Scompare_bigger(struct Sobj* obj1, struct Sobj* obj2) {
        if (obj1->type == SFALSE || obj2->type == SFALSE) {
        Serror_fatal_error("attempt to perform arithmetic on a null value\n");
    }

    if (obj1->type == STRING_OBJ && obj2->type == STRING_OBJ) {
        struct Sstr *str1 = obj1->f_type->f_str;
        struct Sstr *str2 = obj2->f_type->f_str;

        char* a = str1->string;
        char* b = str2->string;

        int size_a = str1->size;
        int size_b = str2->size;

        int result = Scharcmp_bigger(a, b, size_a, size_b);

        return result;
    } else if (obj1->type == LIST_OBJ && obj2->type == LIST_OBJ) {
        int cmp = Slist_cmp(obj1->f_type->f_list, obj2->f_type->f_list);
        if (cmp > 0) {
            return STRUE;
        } else {
            return SFALSE;
        }
    } else if (obj1->type == USER_DATA_OBJ) {
        if (obj1->meta && obj1->meta->mm_gt) {
            return obj1->meta->mm_gt(obj1, obj2)->value->value;
        } else {
            return SFALSE;
        }
    } else if (obj2->type == USER_DATA_OBJ) {
        if (obj2->meta && obj2->meta->mm_gt) {
            return obj2->meta->mm_gt(obj1, obj2)->value->value;
        } else {
            return SFALSE;
        }
    } else {
        if (obj1->value->value > obj2->value->value) {
            return STRUE;
        } else {
            return SFALSE;
        }
    }
}

int Scompare_smaller(struct Sobj* obj1, struct Sobj* obj2) {
    if (obj1->type == SFALSE || obj2->type == SFALSE) {
        Serror_fatal_error("attempt to perform arithmetic on a null value\n");
    }

    if (obj1->type == STRING_OBJ && obj2->type == STRING_OBJ) {
        struct Sstr *str1 = obj1->f_type->f_str;
        struct Sstr *str2 = obj2->f_type->f_str;

        char* a = str1->string;
        char* b = str2->string;

        int size_a = str1->size;
        int size_b = str2->size;

        int result = Scharcmp_smaller(a, b, size_a, size_b);

        return result;
    } else if (obj1->type == LIST_OBJ && obj2->type == LIST_OBJ) {
        int cmp = Slist_cmp(obj1->f_type->f_list, obj2->f_type->f_list);
        if (cmp < 0) {
            return STRUE;
        } else {
            return SFALSE;
        }
    } else if (obj1->type == USER_DATA_OBJ) {
        if (obj1->meta && obj1->meta->mm_lt) {
            return obj1->meta->mm_lt(obj1, obj2)->value->value;
        } else {
            return SFALSE;
        }
    } else if (obj2->type == USER_DATA_OBJ) {
        if (obj2->meta && obj2->meta->mm_lt) {
            obj2->meta->mm_lt(obj1, obj2)->value->value;
        } else {
            return SFALSE;
        }
    } else {
        if (obj1->value->value < obj2->value->value) {
            return STRUE;
        } else {
            return SFALSE;
        }
    }

    return SFALSE;
}

int Scompare_equal(struct Sobj* obj1, struct Sobj* obj2) {
    if (obj1->type == SFALSE || obj2->type == SFALSE) {
        if (obj1->type == SFALSE && obj2->type == SFALSE) {
            return STRUE;
        } else {
            return SFALSE;
        }
    }

    if (obj1->type == STRING_OBJ && obj2->type == STRING_OBJ) {
        struct Sstr *str1 = obj1->f_type->f_str;
        struct Sstr *str2 = obj2->f_type->f_str;

        char* a = str1->string;
        char* b = str2->string;

        int size_a = str1->size;
        int size_b = str2->size;

        int result = Scharcmp_equal(a, b, size_a, size_b);

        return result;
    } else if (obj1->type == LIST_OBJ && obj2->type == LIST_OBJ) {
        int cmp = Slist_cmp(obj1->f_type->f_list, obj2->f_type->f_list);
        if (cmp == 0) {
            return STRUE;
        } else {
            return SFALSE;
        }
    } else if (obj1->type == USER_DATA_OBJ) {
        if (obj1->meta && obj1->meta->mm_eq) {
            return obj1->meta->mm_eq(obj1, obj2)->value->value;
        } else {
            return SFALSE;
        }
    } else if (obj2->type == USER_DATA_OBJ) {
        if (obj2->meta && obj2->meta->mm_eq) {
            return obj2->meta->mm_eq(obj1, obj2)->value->value;
        } else {
            return SFALSE;
        }
    } else {
        if (obj1->value->value == obj2->value->value) {
            return STRUE;
        } else {
            return SFALSE;
        }
    }
}