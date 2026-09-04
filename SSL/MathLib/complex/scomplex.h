#ifndef SCOMPLEX
#define SCOMPLEX

#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../Math/smath.h"

struct Scomplex {
    double real;
    double imag;
};

struct Scomplex* complex(double real, double imag);

struct Scomplex* complex_add(struct Scomplex* a, struct Scomplex* b);
struct Scomplex* complex_sub(struct Scomplex* a, struct Scomplex* b);
struct Scomplex* complex_mul(struct Scomplex* a, struct Scomplex* b);
struct Scomplex* complex_div(struct Scomplex* a, struct Scomplex* b);

double complex_abs(struct Scomplex* a);

#endif // SCOMPLEX
