#include "smath.h"

double __Ssin(double x) {
    while (x > PI)  x -= 2 * PI;
    while (x < -PI) x += 2 * PI;

    double term = x;
    double sum = x;
    double x2 = x * x;

    for (int i = 3, sign = -1; i < 15; i += 2, sign = -sign) {
        term *= x2 / ((i - 1) * i);
        sum += sign * term;
    }
    return sum;
}

double __Scos(double x) {
    while (x > PI)  x -= 2 * PI;
    while (x < -PI) x += 2 * PI;

    double term = 1;
    double sum = 1;
    double x2 = x * x;

    for (int i = 2, sign = -1; i < 14; i += 2, sign = -sign) {
        term *= x2 / ((i - 1) * i);
        sum += sign * term;
    }
    return sum;
}

double __Stan(double x) {
    return __Ssin(x) / __Scos(x);
}

double __Scot(double x) {
    return __Scos(x) / __Ssin(x);
}

double __Sabs(double x) {
    return x < 0 ? -x : x;
}

double __Ssqrt(double x) {
    if (x <= 0) return 0;

    double guess = x;
    for (int i = 0; i < 20; i++) {
        guess = 0.5 * (guess + x / guess);
    }
    return guess;
}

double __Slog(double x) {
    if (x <= 0) return 0;

    double y = (x - 1) / (x + 1);
    double y2 = y * y;

    double sum = 0;
    double term = y;

    for (int i = 1; i < 20; i += 2) {
        sum += term / i;
        term *= y2;
    }

    return 2 * sum;
}

double __Sexp(double x) {
    double sum = 1.0;
    double term = 1.0;

    for (int i = 1; i < 30; i++) {
        term *= x / i;
        sum += term;
    }
    return sum;
}

