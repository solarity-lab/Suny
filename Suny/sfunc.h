#ifndef SFUNC_H
#define SFUNC_H

#include "stype.h"
#include "scode.h"
#include "senvi.h"
#include "sframe.h"

#define MAX_ARGS_SIZE 1024

#define Sobj_get_func(obj) ((obj)->f_type->f_func)

struct Sfunc*
Sfunc_new(void);

struct Sfunc*
Sfunc_set_code(
struct Sfunc *func, 
struct Scode *code);

struct Sfunc*
Sfunc_insert_code(
struct Sfunc *func, 
struct Scode *code);

struct Sobj*
Sobj_set_func
(struct Sfunc *func);

int 
Sfunc_free
(struct Sfunc *func);

struct Sframe*
Sfunc_call_c_api_func
(struct Sframe *frame, 
struct Sobj* f_obj);

struct Sobj*
Sobj_set_closure
(struct Sfunc *func);

#endif