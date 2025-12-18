#include "scall.h"
#include "smem.h"
#include "sdebug.h"

struct Scall_context *
Scall_context_new(void) {
    SDEBUG("[sfunc.c] struct Scall_context *Scall_context_new(void) (building...)\n");
    struct Scall_context *context = Smem_Malloc(sizeof(struct Scall_context));

    context->frame = Sframe_new();

    context->args_index = 0;
    context->code_index = 0;
    context->local_index = 0;
    context->stack_index = 0;

    SDEBUG("[sfunc.c] struct Scall_context *Scall_context_new(void) (done)\n");
    return context;
}

struct Scall_context *
Scall_context_set_func
(struct Scall_context *context, 
    struct Sfunc *func) {
    Sframe_init(context->frame, func->code);
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

        if (!value) {
            __ERROR("Error: value is null\n");
        }

        if (local->is_closure) {
            continue;
        }

        if (value->is_return) {
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
#ifdef DEBUG
    printf("[sfunc.c] int Scall_context_free(struct Scall_context *context) building...)\n");
#endif

    if (context->frame) {
        Scall_context_free_frame(context);
    }

    Smem_Free(context);

#ifdef DEBUG
    printf("[sfunc.c] int Scall_context_free(struct Scall_context *context) (done)\n");
#endif
    return 0;
}

struct Scall_context*
Scall_context_set
(struct Scall_context *context, 
    struct Sframe *frame, 
    struct Sobj* f_obj) 
{
    SDEBUG("[sfunc.c] struct Scall_context* Scall_context_set(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) (building...)\n");
    context->main_frame = frame;

    struct Sframe *f_frame = context->frame;
    struct Scode *f_code = f_obj->f_type->f_func->code;

    Sarray_copy((void**) f_frame->f_globals, (void**) frame->f_globals, frame->f_globals_size);
    f_frame->f_globals_size = frame->f_globals_size;
    f_frame->f_globals_index = frame->f_globals_index;

    f_frame->f_heaps = frame->f_heaps;
    f_frame->f_heap_size = frame->f_heap_size;
    f_frame->f_heap_index = frame->f_heap_index;

    f_frame->f_obj = f_obj;

    f_frame->gc_pool = frame->gc_pool;
    f_frame->f_code = f_code;
    f_frame->f_code_index = 0;
    f_frame->f_label_map = Slabel_map_set_program(f_code);

    SDEBUG("[sfunc.c] struct Scall_context* Scall_context_set(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) (done)\n");
    return context;
}

struct Scall_context*
Scall_context_set_real_func
(struct Scall_context *context, 
    struct Sframe *frame, 
    struct Sobj* f_obj) {
    struct Sobj* class_f = f_obj->prev;

    if (!class_f) {
        return Scall_context_set_frame(context, frame, f_obj);
    }     

    SDEBUG("[sfunc.c] struct Scall_context* Scall_context_set_real_func(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) (building...)\n");

    context = Scall_context_set(context, frame, f_obj);

    struct Sframe *f_frame = context->frame;

    int address = 0;

    struct Sobj** temp = Smem_Calloc(f_obj->f_type->f_func->args_size + 1, sizeof(struct Sobj*));

    int n = f_obj->f_type->f_func->args_size;
    if (n <= 0) {
        Smem_Free(temp);
        return context;
    }

    for (int i = 0; i < n - 1; ++i) {
        struct Sobj* value = Sframe_pop(frame);
        temp[i] = value;
    }

    temp[n - 1] = class_f;
    Sreverse((void **) temp, n);

    for (int i = 0; i < n; ++i) {
        struct Sobj* value = Sframe_store_local(f_frame, address++, temp[i], LOCAL_OBJ);
        value->is_belong_class = 1;
    }

    Smem_Free(temp);
    
    return context;
}

struct Scall_context*
Scall_context_set_frame
(struct Scall_context *context, 
    struct Sframe *frame, 
    struct Sobj* f_obj) {

    context = Scall_context_set(context, frame, f_obj);

    struct Sfunc *func = f_obj->f_type->f_func;

    struct Sframe *f_frame = context->frame;

    int address = 0;

    struct Sobj** temp = Smem_Calloc(f_obj->f_type->f_func->args_size, sizeof(struct Sobj*));    

    for (int i = 0; i < f_obj->f_type->f_func->args_size; ++i) {
        struct Sobj* back = Sframe_back(frame);
        if (back) {
            struct Sobj* value = Sframe_pop(frame);
            temp[i] = value;
        } else {
            temp[i] = null_obj;
        }
    }

    Sreverse((void **) temp, f_obj->f_type->f_func->args_size);
    
    for (int i = 0; i < f_obj->f_type->f_func->args_size; i++) {
        Sframe_store_local(f_frame, func->args_address[i], temp[i], LOCAL_OBJ);
    }

    Smem_Free(temp);

    return context;
}

struct Scall_context*
Scall_context_set_closure
(struct Scall_context *context, 
    struct Sframe *frame, 
    struct Sobj* f_obj)
{
    SDEBUG("[sfunc.c] struct Scall_context* Scall_context_set_closure(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) (building...)\n");
    context = Scall_context_set(context, frame, f_obj);

    struct Sfunc *func = f_obj->f_type->f_func;
    struct Senvi* envi = func->envi;
    struct Sframe *f_frame = context->frame;


    Sarray_copy((void**) f_frame->f_locals, (void**) envi->envi, envi->size);
    
    f_frame->f_locals_size = envi->size;
    f_frame->f_locals_index = envi->size;

    int address = 0;

    struct Sobj** temp = Smem_Calloc(f_obj->f_type->f_func->args_size, sizeof(struct Sobj*));    

    for (int i = 0; i < f_obj->f_type->f_func->args_size; ++i) {
        struct Sobj* back = Sframe_back(frame);
        if (back) {
            struct Sobj* value = Sframe_pop(frame);
            temp[i] = value;
        } else {
            temp[i] = null_obj;
        }
    }

    Sreverse((void **) temp, f_obj->f_type->f_func->args_size);
    
    for (int i = 0; i < f_obj->f_type->f_func->args_size; i++) {
        Sframe_store_local(f_frame, func->args_address[i], temp[i], LOCAL_OBJ);
    }

    Smem_Free(temp);

    SDEBUG("[sfunc.c] struct Scall_context* Scall_context_set_closure(struct Scall_context *context, struct Sframe *frame, struct Sobj* f_obj) (done)\n");
    return context;
}

struct Scall_context *
Scall_context_store_local
(struct Scall_context *context, 
    struct Sobj *local, 
    int address) {
    Sframe_store_local(context->frame, address, local, LOCAL_OBJ);
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

    context = Scall_context_set(context, frame, f_obj);

    int address = 0;

    for (int i = 0; i < f_obj->f_type->f_func->args_size; ++i) {
        Sframe_store_local(f_frame, address++, args[i], LOCAL_OBJ);
    }

    return context;
}