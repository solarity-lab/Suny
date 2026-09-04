#ifndef LIBCOMPLEX_H
#define LIBCOMPLEX_H

#include "C:\Suny-main\Suny\Suny.h"

#include "scomplex.h"

SUNY_API struct Sobj* Sobj_make_complex(struct Scomplex* complex);

SUNY_API struct Sobj* Seval_add_complex(struct Sobj* a, struct Sobj* b);

SUNY_API struct Sobj* Seval_sub_complex(struct Sobj* a, struct Sobj* b);

SUNY_API struct Sobj* Seval_mul_complex(struct Sobj* a, struct Sobj* b);

SUNY_API struct Sobj* Seval_div_complex(struct Sobj* a, struct Sobj* b);

SUNY_API struct Sobj* Sio_print_complex(struct Sobj* obj);

SUNY_API int Sobj_free_complex(struct Sobj* obj);

#endif // LIBCOMPLEX_H