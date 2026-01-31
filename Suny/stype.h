#ifndef STYPE_H
#define STYPE_H

#include "score.h"
#include "sobj.h"
#include "sframe.h"
#include "scode.h"

struct Suserdata {
    void* data;
};

struct Sfunc {
    struct Sfunc* parent;

    struct Scode *code;
    int code_index;
    int code_size;

    struct Sframe* f_frame;

    int is_closure;

    struct Senvi* envi;

    int args_address[DEFAULT_MAX];
    int args_size;
    int args_index;
};

struct Scall_context {
    struct Sfunc *func;
    struct Sframe *frame;
    struct Sframe *main_frame;
    struct Scode *code;
    struct Sobj *ret_obj;
};

struct Sstr {
    char* string;
    int size;
};

struct Slist {
    struct Sobj** array;
    int count;
    int capacity;
};

struct Sbool {
    int _bool;
};

struct Sclass {
    struct Sobj** methods;
    struct Smeta* meta;
    int count;
    int capacity;

    struct Sclass* super_class;
};

struct Stype {
    struct Sfunc            *f_func;            /* function */
    struct Sstr             *f_str;             /* string */
    struct Sclass           *f_class;           /* class */
    struct Slist            *f_list;            /* list */
    struct Sbool            *f_bool;            /* boolen */
    struct Suserdata        *f_userdata;        /* userdata */
};

#define tget_str(o)                 ((o)->f_type->f_str)
#define tget_func(o)                ((o)->f_type->f_func)
#define tget_frame(o)               ((o)->f_type->f_frame)
#define tget_code(o)                ((o)->f_type->f_code)
#define tget_class(o)               ((o)->f_type->f_class)
#define tget_obj(o)                 ((o)->f_type->f_obj)
#define tget_list(o)                ((o)->f_type->f_list)
#define tget_bool(o)                ((o)->f_type->f_bool)
#define tget_call_context(o)        ((o)->f_type->f_call_context)
#define tget_userdata(o)            ((o)->f_type->f_userdata)
#define tget_value(o)               ((o)->f_type->f_value)
#define tget_value(o)               ((o)->f_type->f_value)
#define tget_meta(o)                ((o)->f_type->f_class->meta)
#define tget_type(o)                ((o)->f_type)

struct Stype *
Stype_new(void);

int 
Stype_free
(struct Stype *type);

#endif // STYPE_H