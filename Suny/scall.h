#ifndef SCALL_H
#define SCALL_H

#include "stype.h"
#include "scode.h"
#include "sframe.h"

struct Scall_context; 
struct Sfunc;

struct Scall_context *
Scall_context_new(void);

int
Scall_context_free
(struct Scall_context *context);

int 
Scall_context_free_frame
(struct Scall_context *context);

struct Scall_context*
Scall_context_set_frame
(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj);

struct Scall_context*
Scall_context_set_class_function
(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj);

struct Scall_context*
Scall_context_set_frame_with_args
(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj, struct Sobj** args);

struct Scall_context*
Scall_context_set_closure
(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj);

#endif // SCALL_H