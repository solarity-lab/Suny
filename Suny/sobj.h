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

struct Sgarbarge_obj;
struct Garbage_pool;
struct Smeta;

#define NULL_CODE_PTR Scode_new()

#define GC_HEAD struct Sgarbarge_obj* gc

#define DEFAULT_MAX 1024

typedef unsigned char byte_t;
typedef int address_t;

#define null_obj Sobj_new()
#define true_obj Sobj_make_true()
#define false_obj Sobj_make_false()
#define Svalue(x) Sobj_make_number(x)

#define ValueOf(obj) ((obj)->value->value)
#define Addressof(obj) ((obj)->address)
#define Sizeof(obj) ((obj)->size)
#define Typeof(obj) ((obj)->type)

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
    int ref;
    enum Sobj_t type;
    struct Svalue* value;
    int address;

    struct Sobj *next;
    struct Sobj *prev;
    struct Sobj *f_value; // variable value
    struct Stype *f_type; // real value
    struct Sc_api_func* c_api_func;

    int is_global; // if this object is a global variable
    int is_local; // if this object is a local variable
    int is_closure; // if this object is a closure
    int is_return; // if this object is return in a function
    int is_calle;
    int is_belong_class; // if this object is a member of a class
    int is_super_class_member; // if this object is a member of a super class
    int is_argument; // if this object is a function argument
    int is_shared; // if this object shared for class

    struct Smeta* meta; // metatable
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
