#include "scomplex.h"

struct Scomplex* complex(double real, double imag) {
    struct Scomplex* complex = malloc(sizeof(struct Scomplex));
    complex->real = real;
    complex->imag = imag;
    return complex;    
}
/*
(ai + b) + (ci + d) = (a + b)i + (c + d)


(ai + b) * (ci + d)

= ai * ci + ai * d + b * ci + b * d
= -ac + adi + bci + bd
= (ad + bc)i + (bd - ac)

(ai + b) / (ci + d)

*/

struct Scomplex* complex_add(struct Scomplex* a, struct Scomplex* b) {
    double real = a->real + b->real;
    double imag = a->imag + b->imag;

    return complex(real, imag);
}

struct Scomplex* complex_sub(struct Scomplex* a, struct Scomplex* b) {
    double real = a->real - b->real;
    double imag = a->imag - b->imag;

    return complex(real, imag);
}

struct Scomplex* complex_mul(struct Scomplex* a, struct Scomplex* b) {
    double real = a->real * b->real - a->imag * b->imag;
    double imag = a->imag * b->real + a->real * b->imag;

    return complex(real, imag);
}

struct Scomplex* complex_div(struct Scomplex* a, struct Scomplex* b) {
    double real = (a->real * b->real + a->imag * b->imag) / (b->real * b->real + b->imag * b->imag);
    double imag = (a->imag * b->real - a->real * b->imag) / (b->real * b->real + b->imag * b->imag);
    
    return complex(real, imag);
}

double complex_abs(struct Scomplex* a) {
    double square_a = a->imag * a->imag;
    double square_b = a->real * a->real;

    return __Ssqrt(square_a + square_b);
}