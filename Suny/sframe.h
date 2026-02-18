#ifndef SFRAME_H
#define SFRAME_H

#include "sbuiltin.h"
#include "slabel.h"
#include "sbool.h"
#include "sgc.h"
#include "scompiler.h"
#include "senvi.h"

#define Sframe_load_global_value(frame, address) Sframe_load_global((frame), (address))->f_value

#define POP_OBJ() Sframe_pop(frame)

#define PUSH_OBJ(obj) Sframe_push(frame, obj)

enum Sobj_t;

struct Sframe {
    int f_code_index;
    struct Scode *f_code;

    struct Slabel_map *f_label_map;
    
    struct Sobj **f_stack;
    int f_stack_index;
    int f_stack_size;
    
    struct Sobj **f_locals;
    int f_locals_index;
    int f_locals_size;
    
    struct Sobj **f_globals;
    int f_globals_index;
    int f_globals_size;
    
    struct Sobj **f_consts;
    int f_const_index;
    int f_const_size;
    
    struct Garbage_pool *gc_pool;
    
    struct ScompilerUnit *compiler;
    
    struct Stable *table;
    
    struct Sobj* f_obj; // for closure
    
    struct Senvi* envi;
    
    struct Sframe* parent;

    int GC_THRESHOLD;
};

struct Sframe *
Sframe_new
(void);

int
Sframe_already_defined
(struct Sframe *frame, int address);

struct Sframe *
Sframe_insert_global
(struct Sframe *frame, struct Sobj **f_global, int size);

struct Sframe *
Sframe_insert_local
(struct Sframe *frame, struct Sobj **f_local, int size);

int
Sframe_free
(struct Sframe *frame);

struct Sframe *
Sframe_init
(struct Sframe *frame, struct Scode *code);

struct Sobj *
Sframe_push
(struct Sframe *frame, struct Sobj *obj);

struct Sobj *
Sframe_pop
(struct Sframe *frame);

struct Sobj *
Sframe_back
(struct Sframe *frame);

struct Sobj *
Sframe_load_const
(struct Sframe *frame, int index);

struct Sobj *
Sframe_store_const
(struct Sframe *frame, int index, struct Sobj *obj);

struct Sobj*
Sframe_store_closure
(struct Sframe *frame, int address, struct Sobj *obj, enum Sobj_t type);

struct Sobj *
Sframe_load_closure
(struct Sframe *frame, int address);

struct Sobj*
Sframe_store_global
(struct Sframe *frame, int address, struct Sobj *obj, enum Sobj_t type);

struct Sobj *
Sframe_load_global
(struct Sframe *frame, int address);

struct Sobj *
Sframe_store_local
(struct Sframe *frame, int address, struct Sobj *obj, enum Sobj_t type);

struct Sobj *
Sframe_load_local
(struct Sframe *frame, int address);

struct Sobj *
Sframe_load_c_api_func
(struct Sframe *frame, void* func, int address, char* name, int args_size);

void*
Sframe_find_c_api_func
(struct Sframe *frame, int address);

struct Sframe*
Sframe_push_number(struct Sframe* frame, double number);

struct Sframe*
Sframe_push_string(struct Sframe* frame, char* string, int size);

struct Sframe*
Sframe_push_bool(struct Sframe* frame, int b);

struct Sframe*
Sframe_push_null(struct Sframe* frame);

struct Sobj*
Sframe_get_top(struct Sframe* frame);

struct Sobj*
Sframe_true_pop(struct Sframe* frame);

struct Sframe*
Sframe_call_c_api_func(struct Sframe* frame, void* func);

#endif // SFRAME_H