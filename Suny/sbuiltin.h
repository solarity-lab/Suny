#ifndef SBUILTIN_H
#define SBUILTIN_H

#include "score.h"
#include "stool.h"
#include "slist.h"
#include "sio.h"
#include "scopy.h"

struct Sframe;
struct ScompilerUnit;
struct Stable;
struct Scall_context;

struct Sframe *Svm_run_call_context(struct Scall_context *context);

#define call_func(func, frame) ((struct Sobj* (*)(struct Sframe*)) (func))((frame));

#define get_c_api_func(f_obj) (f_obj)->c_api_func->func

struct Sc_api_func {
    void* func;
    char* name;
    int address;
    int args_size;
};

struct Sc_api_func* 
Sc_api_func_set
(   
    void* func, 
    char* name, 
    int address,
    int args_size
);

struct Sc_api_func* 
Sc_api_func_set
(void* func, char* name, int address, int args_size);

SUNY_API struct Sobj* Sprint(struct Sframe* frame);

SUNY_API struct Sobj* Sexit(struct Sframe* frame);

SUNY_API struct Sobj* Sputs(struct Sframe* frame);

SUNY_API struct Sobj* Sread(struct Sframe* frame);

SUNY_API struct Sobj* Ssize(struct Sframe* frame);

SUNY_API struct Sobj* Spush(struct Sframe* frame);

SUNY_API struct Sobj* Spop(struct Sframe* frame);

SUNY_API struct Sobj* Sload_dll(struct Sframe *frame);

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
