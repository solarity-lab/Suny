#include "MathLib.h"

SUNY_API struct Sobj* Scomplex(struct Sframe* frame) {
    struct Sobj* real_obj = Sframe_pop(frame);
    struct Sobj* imag_obj = Sframe_pop(frame);

    double real = ValueOf(real_obj);
    double imag = ValueOf(imag_obj);

    struct Sobj* complex_obj = Sobj_make_complex(complex(real, imag));

    MOVETOGC(real_obj, frame->gc_pool);
    MOVETOGC(imag_obj, frame->gc_pool);
    
    return complex_obj;
}

SUNY_API struct Sobj* Ssqrt(struct Sframe* frame) {
    struct Sobj* obj = Sframe_pop(frame);
    MOVETOGC(obj, frame->gc_pool);
    return Sobj_make_number(__Ssqrt(ValueOf(obj)));
}   

SUNY_API struct Sobj* Sabs(struct Sframe* frame) {
    struct Sobj* obj = Sframe_pop(frame);
    MOVETOGC(obj, frame->gc_pool);
    return Sobj_make_number(__Sabs(ValueOf(obj)));
}

SUNY_API struct Sobj* Slog(struct Sframe* frame) {
    struct Sobj* b = Sframe_pop(frame);
    struct Sobj* a = Sframe_pop(frame);
    
    MOVETOGC(a, frame->gc_pool);
    MOVETOGC(b, frame->gc_pool);

    return Sobj_make_number(__Slog(ValueOf(b)) / __Slog(ValueOf(a)));
}

SUNY_API struct Sobj* Ssin(struct Sframe* frame) {
    struct Sobj* obj = Sframe_pop(frame);
    MOVETOGC(obj, frame->gc_pool);
    return Sobj_make_number(__Ssin(ValueOf(obj)));
}

SUNY_API struct Sobj* Scos(struct Sframe* frame) {
    struct Sobj* obj = Sframe_pop(frame);
    MOVETOGC(obj, frame->gc_pool);
    return Sobj_make_number(__Scos(ValueOf(obj)));
}

SUNY_API struct Sobj* Stan(struct Sframe* frame) {
    struct Sobj* obj = Sframe_pop(frame);
    MOVETOGC(obj, frame->gc_pool);
    return Sobj_make_number(__Stan(ValueOf(obj)));
}

SUNY_API struct Sobj* Scot(struct Sframe* frame) {
    struct Sobj* obj = Sframe_pop(frame);
    MOVETOGC(obj, frame->gc_pool);
    return Sobj_make_number(__Scot(ValueOf(obj)));
}

SUNY_API struct Sframe* Smain(struct Sframe* frame, struct Stable *table) {
    Sinitialize_variables(frame, table, "e", 18, Svalue(E_CONST));
    Sinitialize_variables(frame, table, "pi", 19, Svalue(PI_CONST));

    Sinitialize_c_api_func(frame, table, 20, 1, "complex", Scomplex);
    Sinitialize_c_api_func(frame, table, 21, 1, "sqrt", Ssqrt);
    Sinitialize_c_api_func(frame, table, 22, 1, "abs", Sabs);
    Sinitialize_c_api_func(frame, table, 23, 1, "log", Slog);
    Sinitialize_c_api_func(frame, table, 24, 1, "sin", Ssin);
    Sinitialize_c_api_func(frame, table, 25, 1, "cos", Scos);
    Sinitialize_c_api_func(frame, table, 26, 1, "tan", Stan);
    Sinitialize_c_api_func(frame, table, 27, 1, "cot", Scot);
    
    return frame;
}