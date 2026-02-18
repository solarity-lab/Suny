#include "slogic.h"

int Slogic_AND(struct Sobj* a, struct Sobj* b) {
    if (!a || !b) return -1;

    Number_t n1 = ValueOf(a);
    Number_t n2 = ValueOf(b);

    return Land(n1, n2);
}

int Slogic_OR(struct Sobj* a, struct Sobj* b) {
    if (!a || !b) return -1;

    Number_t n1 = ValueOf(a);
    Number_t n2 = ValueOf(b);

    return Lor(n1, n2);
}

int Slogic_IS(struct Sobj* a, struct Sobj* b) {
    if (!a || !b) return -1;

    return Lis(a, b);
}

int Slogic_NOT(struct Sobj* obj) {
    if (!obj) return -1;
    return Lnot(ValueOf(obj));
}
