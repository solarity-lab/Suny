#ifndef SOBJ_H
#define SOBJ_H

#include "score.h"
#include "stype.h"
#include "sclass.h"
#include "sstr.h"
#include "sfunc.h"
#include "scall.h"
#include "score.h"
#include "smeta.h"
#include "suserdata.h"

/*
    This file define the main value of 
*/

struct Sgarbarge_obj;
struct Garbage_pool;
struct Smeta;

#define null_obj Sobj_new()
#define true_obj Sobj_make_true()
#define false_obj Sobj_make_false()
#define Svalue(x) Sobj_make_number(x)

#define ValueOf(obj) ((obj)->value->value)
#define AddressOf(obj) ((obj)->address)
#define StringOf(obj) ((obj)->f_type->f_str->string)
#define TypeOf(obj) ((obj)->type)

enum Sobj_t {
    NUMBER_OBJ,
    GLOBAL_OBJ,
    BUILTIN_OBJ,
    LOCAL_OBJ,
    STRING_OBJ,
    MAP_OBJ,
    CHAR_OBJ,
    LIST_OBJ,
    USER_DATA_OBJ,
    CLASS_OBJ,
    INSTANCE_OBJ,
    CLOSURE_OBJ,
    TRUE_OBJ,
    FALSE_OBJ,
    FUNC_OBJ,
    NULL_OBJ,
    NONE,
};

struct Svalue {
    double value;
};

struct Sobj {
    int ref;                            /* ref count */

    enum Sobj_t type;                   /* type of object */
    struct Svalue* value;               /* main value use for number */
    struct Stype *f_type;               /* real value */
    
    int address;                        /* address of variable*/
    struct Sobj *f_value;               /* variable value */

    struct Sc_api_func* c_api_func;     /* c api function for userdata */

    int is_global;                      /* if this object is a global variable */
    int is_local;                       /* if this object is a local variable */
    int is_closure;                     /* if this object is a closure */
    int is_return;                      /* if this object is return in a function */
    int is_extends;                     /* if this object is a member of a super class */
    int is_shared;                      /* if this object shared for class */

    struct Smeta* meta;                 /* meta */

    struct Sobj* prev;                  /* the object that point to this object */
};

struct Svalue*
Svalue_new(void);

struct Sobj*
Sobj_new(void);

int
Sobj_free
(struct Sobj* obj);

int
Sobj_free_objs
(struct Sobj** objs, int size);

void*
Sobj_get_obj
(struct Sobj* obj, enum Sobj_t type);

struct Sobj*
Sobj_make_null(void);

struct Sobj*
Sobj_make_number
(double value);

struct Sobj*
Sobj_make_type(enum Sobj_t type);

#endif // SOBJ_H
