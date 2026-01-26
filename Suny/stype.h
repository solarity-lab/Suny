#ifndef STYPE_H
#define STYPE_H

#include "score.h"
#include "sobj.h"
#include "sframe.h"
#include "scode.h"
#include "max.h"

typedef void (*SunyGcFn)(void* data);
typedef void (*SunyToStringFn)(void* data, char* buf, size_t bufsize);

struct Scall_context;
struct Sfunc;
struct Sstr;
struct Slist;
struct Sbool;
struct Sclass;
struct Stype;
struct Suserdata;
struct Smeta;

struct Suserdata {
    void* data;
    struct Stype* type;
};

struct Sfunc {
    struct Sfunc* parent;
    struct Scode *code;
    int code_index;
    int code_size;
    struct Sobj **params;
    struct Sframe* f_frame;
    int is_closure;
    struct Sobj* cf; // closure function
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
    
    int args_index;
    int code_index;
    int stack_index;
    int local_index;

    struct Sobj *ret_obj;
};

struct Sstr {
    unsigned char buff[MAX_IDENTIFIER_SIZE];
    int size;
    char* string;
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
    struct Sfunc            *f_func;
    struct Sstr             *f_str;
    struct Svalue           *f_value;
    struct Sframe           *f_frame;
    struct Scode            *f_code;
    struct Sclass           *f_class;
    struct Sobj             *f_obj;
    struct Slist            *f_list;
    struct Sbool            *f_bool;
    struct Scall_context    *f_call_context;
    struct Suserdata        *f_userdata;
};

#define tget_str(o) ((o)->f_type->f_str)

#define tget_func(o) ((o)->f_type->f_func)

#define tget_frame(o) ((o)->f_type->f_frame)

#define tget_code(o) ((o)->f_type->f_code)

#define tget_class(o) ((o)->f_type->f_class)

#define tget_obj(o) ((o)->f_type->f_obj)

#define tget_list(o) ((o)->f_type->f_list)

#define tget_bool(o) ((o)->f_type->f_bool)

#define tget_call_context(o) ((o)->f_type->f_call_context)

#define tget_userdata(o) ((o)->f_type->f_userdata)

#define tget_value(o) ((o)->f_type->f_value)

#define tget_value(o) ((o)->f_type->f_value)

#define tget_meta(o) ((o)->f_type->f_class->meta)

#define tget_type(o) ((o)->f_type)

struct Stype *
Stype_new(void);

int 
Stype_free
(struct Stype *type);

#endif // STYPE_H