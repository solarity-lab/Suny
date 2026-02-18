#ifndef SUSERDATA_H
#define SUSERDATA_H

#include "stype.h"

#define get_userdata(obj) (obj->f_type->f_userdata->data)

struct Suserdata* 
Suserdata_new(void);

struct Suserdata* 
Suserdata_set
(void* data);

struct Sobj* 
Sobj_make_userdata
(void* data);

int Suserdata_free(struct Suserdata* userdata);

#endif  // SUSERDATA_H