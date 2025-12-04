#ifndef SCALL_H
#define SCALL_H

#include "stype.h"
#include "scode.h"
#include "sframe.h"

struct Scall_context; 
struct Sfunc;

struct Scall_context *
Scall_context_new(void);

struct Scall_context *
Scall_context_store_local
(struct Scall_context *context, 
    struct Sobj *local, 
    int address);

struct Scall_context*
Scall_context_set_func
(struct Scall_context *context, 
    struct Sfunc *func);

struct Scall_context*
Scall_context_set_frame
(struct Scall_context *context, 
    struct Sframe *frame, 
    struct Sobj* f_obj);

struct Scall_context*
Scall_context_set_real_func
(struct Scall_context *context, 
    struct Sframe *frame, 
    struct Sobj* f_obj);

int 
Scall_context_free
(struct Scall_context *context);

struct Scall_context*
Scall_context_set_frame_with_args
(struct Scall_context *context, 
    struct Sframe *frame, 
    struct Sobj* f_obj, 
    struct Sobj** args);

int 
Scall_context_free_frame
(struct Scall_context *context);

struct Scall_context*
Scall_context_set_closure
(struct Scall_context *context, 
    struct Sframe *frame, 
    struct Sobj* f_obj);

struct Scall_context*
Scall_context_set_closure
(struct Scall_context *context, 
    struct Sframe *frame, 
    struct Sobj* f_obj);

#endif // SCALL_H