#ifndef SCLASS_H
#define SCLASS_H

#include "stype.h"
#include "score.h"
#include "scopy.h"
#include "sframe.h"

#define CLASS_METHOD_NOT_FOUND -1
#define __INIT__ADDRESS 20
#define __ADD__ADDRESS 21
#define __SUB__ADDRESS 22
#define __MUL__ADDRESS 23
#define __DIV__ADDRESS 24
#define __TO_STR__ADDRESS 25

struct Sclass*
Sclass_new
(void);

int 
Sclass_free
(struct Sclass* sclass);

struct Sclass* 
Sclass_store_object
(struct Sclass* sclass, struct Sframe* frame, int address);

struct Sclass* 
Sclass_store_local_obj
(struct Sclass* sclass, struct Sframe* frame, struct Sobj* value, int address);

struct Sobj* 
Sclass_get_object
(struct Sclass* sclass, int address);

struct Sobj* 
Sclass_push_obj
(struct Sclass* sclass, struct Sobj* obj);

struct Sobj*
Sobj_make_class(struct Sclass* sclass);

struct Sobj*
Sobj_copy_class(struct Sclass* sclass);

struct Sclass* 
Sclass_extends_class
(struct Sclass* sclass, struct Sclass* super_class);

struct Sclass*
Sclass_shared_class
(struct Sclass* sclass, struct Sclass* super_class);

struct Sframe*
Sclass_call(struct Sframe* frame, struct Sobj* f_obj);

#endif // SCLASS_H
