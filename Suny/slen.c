#include "slen.h"

double Slen(struct Sobj* obj) {
    if (TypeOf(obj) == LIST_OBJ) {
        return tget_list(obj)->count;
    } else if (TypeOf(obj) == STRING_OBJ) {
        return tget_str(obj)->size;
    }

    return 0;
}