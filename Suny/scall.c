#include "scall.h"
#include "smem.h"
#include "sdebug.h"

int store_args(struct Sobj** temp, struct Sframe *frame, int args_size) {
    for (int i = 0; i < args_size; i++) {
        struct Sobj* back = Sframe_back(frame);
        if (back) {
            struct Sobj* value = Sframe_pop(frame);
            temp[i] = value;
        } else temp[i] = true_obj;
    }
    return 0;
}

int store_args_to_func(struct Sframe *f_frame, struct Sfunc *func, struct Sobj** temp) {
    for (int i = 0; i < func->args_size; i++) {
        if (!temp[i]) continue;
        Sframe_store_local(f_frame, func->args_address[i], temp[i], LOCAL_OBJ);
    }
    return 0;
}

int store_args_function_class(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj, struct Sobj* class_f) {
    struct Sframe *f_frame = context->frame;
    struct Sfunc *func = f_obj->f_type->f_func;
    
    struct Sobj** temp = Smem_Calloc(func->args_size, sizeof(struct Sobj*));

    store_args(temp, frame, func->args_size - 1);

    if (func->args_size > 0) temp[func->args_size - 1] = class_f;
    
    Sreverse((void **) temp, func->args_size);
    
    store_args_to_func(f_frame, func, temp);
    
    Smem_Free(temp);
    return 0;
}

int store_args_function(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) {
    struct Sfunc *func = f_obj->f_type->f_func;
    struct Sframe *f_frame = context->frame;

    if (func->args_size == 0) return 0;

    struct Sobj** temp = Smem_Calloc(func->args_size, sizeof(struct Sobj*));    

    store_args(temp, frame, func->args_size);

    Sreverse((void **) temp, f_obj->f_type->f_func->args_size);

    store_args_to_func(f_frame, func, temp);
    
    Smem_Free(temp);
    
    return 0;
}

int store_closure_args(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) {
    struct Sfunc *func = f_obj->f_type->f_func;
    struct Senvi* envi = func->envi;
    struct Sframe *f_frame = context->frame;

    Sarray_copy((void**) f_frame->f_locals, (void**) envi->envi, envi->size);
    
    f_frame->f_locals_size = envi->size;
    f_frame->f_locals_index = envi->size;
    
    struct Sobj** temp = Smem_Calloc(func->args_size, sizeof(struct Sobj*));    
    
    store_args(temp, frame, func->args_size);
    
    Sreverse((void **) temp, func->args_size);
    
    store_args_to_func(f_frame, func, temp);
    
    Smem_Free(temp);
    return 0;
}

struct Scall_context *
Scall_context_new(void) {
    SDEBUG("[sfunc.c] struct Scall_context *Scall_context_new(void) (building...)\n");
    struct Scall_context *context = Smem_Malloc(sizeof(struct Scall_context));
    
    context->frame = Sframe_new();
    
    SDEBUG("[sfunc.c] struct Scall_context *Scall_context_new(void) (done)\n");
    return context;
}

int 
Scall_context_free_frame
(struct Scall_context *context) {
    SDEBUG("[sfunc.c] int Scall_context_free_frame(struct Scall_context *context) (building...)\n");
    struct Sframe *frame = context->frame;

    for (int i = 0; i < frame->f_locals_size; ++i) {
        struct Sobj *local = frame->f_locals[i];
        struct Sobj *value = local->f_value;

        if (!value) __ERROR("Error: value is null\n");

        if (local->is_closure) continue; 

        if (value->is_return) {

            _SUNYDECREF(value);
            Sobj_free(local);
            continue;
        }

        _SUNYDECREF(value);
        MOVETOGC(value, frame->gc_pool);

        Sobj_free(local);
    }

    Sobj_free_objs(frame->f_stack, frame->f_stack_index);

    Sobj_free_objs(frame->f_consts, frame->f_const_index);

    Slabel_map_free(frame->f_label_map);

    Smem_Free(frame->f_locals); 
    Smem_Free(frame->f_globals);
    Smem_Free(frame);

    SDEBUG("[sfunc.c] int Scall_context_free_frame(struct Scall_context *context) (done)\n");
    return 0;   
}

int 
Scall_context_free
(struct Scall_context *context) {
    SDEBUG("[sfunc.c] int Scall_context_free(struct Scall_context *context) (building...)\n");
    if (context->frame) Scall_context_free_frame(context);

    Smem_Free(context);
    
    SDEBUG("[sfunc.c] int Scall_context_free(struct Scall_context *context) (done)\n");
    return 0;
}

struct Scall_context*
Scall_context_set
(struct Scall_context *context, 
    struct Sframe *frame, 
    struct Sobj* f_obj) {
    SDEBUG("[sfunc.c] struct Scall_context* Scall_context_set(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) (building...)\n");
    context->main_frame = frame;
    
    struct Sframe *f_frame = context->frame;
    
    struct Scode *f_code = f_obj->f_type->f_func->code;
    
    Sarray_copy((void**) f_frame->f_globals, (void**) frame->f_globals, frame->f_globals_size);
    
    f_frame->f_globals_size = frame->f_globals_size;
    f_frame->f_globals_index = frame->f_globals_index;
        
    f_frame->gc_pool = frame->gc_pool;

    f_frame->f_code = f_code;
    f_frame->f_label_map = Slabel_map_set_program(f_code);
    
    SDEBUG("[sfunc.c] struct Scall_context* Scall_context_set(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) (done)\n");
    return context;
}

struct Scall_context*
Scall_context_set_class_function
(struct Scall_context *context, 
struct Sframe *frame, 
struct Sobj* f_obj) {
    struct Sobj* class_f = f_obj->prev;
    if (!class_f) return Scall_context_set_frame(context, frame, f_obj);
    SDEBUG("[sfunc.c] struct Scall_context* Scall_context_set_class_function(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) (building...)\n");
    context = Scall_context_set(context, frame, f_obj);
    store_args_function_class(context, frame, f_obj, class_f);
    SDEBUG("[sfunc.c] struct Scall_context* Scall_context_set_class_function(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) (done)\n");
    return context;
}

struct Scall_context*
Scall_context_set_frame
(struct Scall_context *context, 
    struct Sframe *frame, 
    struct Sobj* f_obj) {
    context = Scall_context_set(context, frame, f_obj);
    store_args_function(context, frame, f_obj);
    return context;
}

struct Scall_context*
Scall_context_set_closure
(struct Scall_context *context, 
    struct Sframe *frame, 
    struct Sobj* f_obj) {
    SDEBUG("[sfunc.c] struct Scall_context* Scall_context_set_closure(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) (building...)\n");
    context = Scall_context_set(context, frame, f_obj);
    store_closure_args(context, frame, f_obj);
    SDEBUG("[sfunc.c] struct Scall_context* Scall_context_set_closure(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) (done)\n");
    return context;
}

struct Scall_context*
Scall_context_set_frame_with_args
(struct Scall_context *context, 
    struct Sframe *frame, 
    struct Sobj* f_obj, 
    struct Sobj** args) {
    context->main_frame = frame;
    struct Sframe *f_frame = context->frame;
    struct Sfunc* func = f_obj->f_type->f_func;
    context = Scall_context_set(context, frame, f_obj);
    store_args_to_func(f_frame, func, args);
    return context;
}
