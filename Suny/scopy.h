#ifndef SCOPY_H
#define SCOPY_H

#include "sobj.h"
#include "sclass.h"

struct Sobj* Sobj_deep_copy(struct Sobj* obj);

struct Sobj* Sobj_shallow_copy(struct Sobj* obj);

struct Scode* Scode_deep_copy(struct Scode* code);

struct Sfunc* Sfunc_deep_copy(struct Sfunc* func);

struct Slist* Slist_copy(struct Slist* list);

struct Sclass* Sclass_copy(struct Sclass* sclass);

#endif // SCOPY_H