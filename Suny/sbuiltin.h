#ifndef SBUILTIN_H
#define SBUILTIN_H

#include "score.h"
#include "stool.h"
#include "slist.h"
#include "sio.h"
#include "scopy.h"
#include "sc_api_func.h"

struct Sc_api_func;

SUNY_API struct Sobj* Sprint(struct Sframe* frame);

SUNY_API struct Sobj* Sexit(struct Sframe* frame);

SUNY_API struct Sobj* Sputs(struct Sframe* frame);

SUNY_API struct Sobj* Sread(struct Sframe* frame);

SUNY_API struct Sobj* Ssize(struct Sframe* frame);

SUNY_API struct Sobj* Spush(struct Sframe* frame);

SUNY_API struct Sobj* Spop(struct Sframe* frame);

SUNY_API struct Sobj* Sload(struct Sframe *frame);

SUNY_API struct Sobj* Sisdigit_builtin(struct Sframe *frame);

SUNY_API struct Sobj* Srange(struct Sframe *frame);

SUNY_API struct Sobj* Scopy(struct Sframe *frame);

SUNY_API struct Sobj* Slist_cast(struct Sframe* frame);

SUNY_API struct Sobj* Sstring_cast(struct Sframe* frame);

SUNY_API struct Sobj* Sint_cast(struct Sframe* frame);

SUNY_API struct Sobj* Sfloat_cast(struct Sframe* frame);

SUNY_API struct Sobj* Sbool_cast(struct Sframe* frame);

SUNY_API struct Sobj* Stype(struct Sframe* frame);

SUNY_API struct Sobj* Ssystem(struct Sframe* frame);

#endif // SBUILTIN_H
