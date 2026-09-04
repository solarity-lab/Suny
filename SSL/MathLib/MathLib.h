#ifndef MATHLIB_H
#define MATHLIB_H

#include "./complex/LibComplex.h"
#include "./Math/smath.h"

#include "C:\Suny-main\Suny\Suny.h"

#define E_CONST 2.7182818284590452354
#define PI_CONST 3.1415926535897932385

SUNY_API struct Sobj* Scomplex(struct Sframe* frame);

SUNY_API struct Sobj* Ssqrt(struct Sframe* frame);
SUNY_API struct Sobj* Sabs(struct Sframe* frame);
SUNY_API struct Sobj* Slog(struct Sframe* frame);

SUNY_API struct Sobj* Ssin(struct Sframe* frame);
SUNY_API struct Sobj* Scos(struct Sframe* frame);
SUNY_API struct Sobj* Stan(struct Sframe* frame);
SUNY_API struct Sobj* Scot(struct Sframe* frame);

#endif // MATHLIB_H