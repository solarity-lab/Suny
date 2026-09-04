#include "LibComplex.h"

SUNY_API struct Sobj* Sobj_make_complex(struct Scomplex* complex) {

    struct Sobj* obj = Sobj_make_userdata(complex);

    obj->meta = Smeta_new();

    Smeta_set(obj, "__add__",       Seval_add_complex);
    Smeta_set(obj, "__sub__",       Seval_sub_complex);
    Smeta_set(obj, "__mul__",       Seval_mul_complex);
    Smeta_set(obj, "__div__",       Seval_div_complex);
    Smeta_set(obj, "__tostring__",  Sio_print_complex);
    Smeta_set(obj, "__free__",      Sobj_free_complex);

    return obj;
}

SUNY_API struct Sobj* Seval_add_complex(struct Sobj* a, struct Sobj* b) {
    struct Scomplex* ca = get_userdata(a);
    struct Scomplex* cb = get_userdata(b);

    struct Scomplex* comp = complex_add(ca, cb);

    return Sobj_make_complex(comp);
}

SUNY_API struct Sobj* Seval_sub_complex(struct Sobj* a, struct Sobj* b) {
    struct Scomplex* ca = get_userdata(a);
    struct Scomplex* cb = get_userdata(b);

    struct Scomplex* comp = complex_sub(ca, cb);

    return Sobj_make_complex(comp);
}

SUNY_API struct Sobj* Seval_mul_complex(struct Sobj* a, struct Sobj* b) {
    struct Scomplex* ca = get_userdata(a);
    struct Scomplex* cb = get_userdata(b);

    struct Scomplex* comp = complex_mul(ca, cb);

    return Sobj_make_complex(comp);
}

SUNY_API struct Sobj* Seval_div_complex(struct Sobj* a, struct Sobj* b) {
    struct Scomplex* ca = get_userdata(a);
    struct Scomplex* cb = get_userdata(b);

    struct Scomplex* comp = complex_div(ca, cb);

    return Sobj_make_complex(comp);
}

SUNY_API struct Sobj* Sio_print_complex(struct Sobj* obj) {
    struct Scomplex* comp = get_userdata(obj);
    printf("%lf + %lf * i", comp->real, comp->imag);
    return obj;
}

SUNY_API int Sobj_free_complex(struct Sobj* obj) {
    struct Scomplex* comp = get_userdata(obj);
    free(comp);
    return 0;
}