#include "sbool.h"

SUNY_API struct Sobj* Sobj_make_true() {
    struct Sobj* obj = Sobj_new();
    obj->type = TRUE_OBJ;
    obj->value->value = 1;
    return obj;
}
 
SUNY_API struct Sobj* Sobj_make_false() {
    struct Sobj* obj = Sobj_new();
    obj->type = FALSE_OBJ;
    obj->value->value = 0;
    return obj;
}

SUNY_API struct Sobj* Sobj_make_bool(int value) {
    return value ? Sobj_make_true() : Sobj_make_false();
}