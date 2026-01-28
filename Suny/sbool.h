#ifndef SBOOL_H
#define SBOOL_H

#include "sobj.h"
#include "stype.h"

SUNY_API struct Sobj* Sobj_make_true();
SUNY_API struct Sobj* Sobj_make_false();

SUNY_API struct Sobj* Sobj_make_bool(int value);

#endif // SBOOL_H