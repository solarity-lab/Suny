#include "sframe.h"
#include "sdebug.h"
#include "smem.h"

struct Sframe *
Sframe_new(void) {
    struct Sframe *frame = Smem_Calloc(1, sizeof(struct Sframe));

    frame->f_stack   = Smem_Calloc(DEFAULT_MAX, sizeof(struct Sobj *));
    frame->f_locals  = Smem_Calloc(DEFAULT_MAX, sizeof(struct Sobj *));
    frame->f_globals = Smem_Calloc(DEFAULT_MAX, sizeof(struct Sobj *));

    frame->f_stack_index = 0;
    frame->f_stack_size = 1024;
    frame->f_globals_index = 0;
    frame->f_globals_size = 0;
    frame->f_locals_index = 0;
    frame->f_locals_size = 0;

    frame->GC_THRESHOLD = POOL_SIZE_LIMIT;

    return frame;
}

int
Sframe_free
(struct Sframe *frame) {
    Sobj_free_objs(frame->f_stack, frame->f_stack_index);
    Sobj_free_objs(frame->f_locals, frame->f_locals_index);
    Sobj_free_objs(frame->f_globals, frame->f_globals_index);

    if (frame->f_label_map) {
        Smem_Free(frame->f_label_map);
    }

    Sgc_free_pool(frame->gc_pool);

    Smem_Free(frame);
    return 0;
}

struct Sframe *
Sframe_init
(struct Sframe *frame, struct Scode *code) {
    frame->f_code = code;
    frame->f_code_index = 0;
    frame->f_label_map = Slabel_map_set_program(code);
    return frame;
}

struct Sobj *
Sframe_push(struct Sframe *frame, struct Sobj *obj) {
    if (!obj)
        __ERROR("null object\n");

    if (frame->f_stack_index >= frame->f_stack_size) {
        frame->f_stack_size *= 2;
        frame->f_stack = Smem_Realloc(frame->f_stack, frame->f_stack_size * sizeof(struct Sobj *));
    }

    _SUNYINCREF(obj);

    frame->f_stack[frame->f_stack_index++] = obj;
    return obj;
}

struct Sobj *
Sframe_pop
(struct Sframe *frame) {
    if (frame->f_stack_index <= 0) {
        __ERROR("Error frame.c: stack underflow stack index: %d\n", frame->f_stack_index);
        return NULL;
    };

    struct Sobj *obj = frame->f_stack[--frame->f_stack_index];

    if (!obj) {
        __ERROR("Error frame.c: obj is null stack index: %d\n", frame->f_stack_index);
        return NULL;
    }

    _SUNYDECREF(obj);
    return obj;
}

struct Sobj *
Sframe_back
(struct Sframe *frame) {
    if (frame->f_stack_index <= 0) {
        return NULL;
    }

    struct Sobj *obj = frame->f_stack[frame->f_stack_index - 1];

    if (!obj) {
        return NULL;
    }

    return obj;
}

struct Sobj*
Sframe_store_global
(struct Sframe *frame, int address, struct Sobj *obj, enum Sobj_t type) {
    if (!frame || !obj) {
        return NULL;
    }

    for (int i = 0; i < frame->f_globals_size; i++) {
        if (frame->f_globals[i]->address == address) {
            struct Sobj *old = frame->f_globals[i]->f_value;

            if (old) {
                _SUNYDECREF(old);
                MOVETOGC(old, frame->gc_pool);
            }

            _SUNYINCREF(obj);

            frame->f_globals[i]->f_value = obj;
            frame->f_globals[i]->type = type;
            frame->f_globals[i]->address = address;

            return frame->f_globals[i];
        }
    }

    struct Sobj *global = Sobj_new();

    global->type = type;

    _SUNYINCREF(obj);
    global->f_value = obj;

    global->f_value->address = address;
    global->f_value->is_global = 1;
    global->address = address;

    frame->f_globals[frame->f_globals_index++] = global;
    frame->f_globals_size++;

    return global;
}

struct Sobj *
Sframe_load_global
(struct Sframe *frame, int address) {
    struct Sobj *load = NULL;

    for (int i = 0; i < frame->f_globals_size; i++) {
        if (frame->f_globals[i]->address == address) {
            load = frame->f_globals[i];
            break;
        }
    }

    if (!load) {
        __ERROR("Error frame.c: global not found address: %d\n", address);
        return NULL;
    }

    return load;
}

struct Sobj*
Sframe_store_local
(struct Sframe *frame, int address, struct Sobj *obj, enum Sobj_t type) {

    for (int i = 0; i < frame->f_locals_size; i++) {
        if (frame->f_locals[i]->address == address && !frame->f_locals[i]->is_closure) {
            struct Sobj *old = frame->f_locals[i]->f_value;

            if (old) {
                _SUNYDECREF(old);
                MOVETOGC(old, frame->gc_pool);
            }

            _SUNYINCREF(obj);

            frame->f_locals[i]->f_value = obj;
            frame->f_locals[i]->type = type;

            return frame->f_locals[i];
        }
    }

    for (int i = 0; i < frame->f_globals_size; i++) {
        if (frame->f_globals[i]->address == address && !frame->f_globals[i]->is_closure) {
            struct Sobj *old = frame->f_globals[i]->f_value;

            _SUNYDECREF(old);
            MOVETOGC(old, frame->gc_pool);

            _SUNYINCREF(obj);

            frame->f_globals[i]->f_value = obj;
            frame->f_globals[i]->type = type;

            return frame->f_globals[i];
        }
    }

    struct Sobj *local = Sobj_new();
    
    _SUNYINCREF(obj);

    local->type = LOCAL_OBJ;
    local->f_value = obj;
    local->f_value->address = address;
    local->address = address;

    frame->f_locals[frame->f_locals_index++] = local;
    frame->f_locals_size++;

    return local;
}

struct Sobj *
Sframe_load_local
(struct Sframe *frame, int address) {
    struct Sobj *load = NULL;
    
    for (int i = 0; i < frame->f_locals_size; i++) {
        if (frame->f_locals[i]->address == address) {
            load = frame->f_locals[i];
            break;
        }
    }

    if (!load) {
        for (int i = 0; i < frame->f_globals_size; i++) {
            if (frame->f_globals[i]->address == address) {
                load = frame->f_globals[i];
                break;
            }
        }
    }

    if (!load) {
        __ERROR("Error frame.c: local not found address: %d\n", address);
        return NULL;
    }

    return load;
}

struct Sframe *
Sframe_insert_global
(struct Sframe *frame, struct Sobj **f_global, int size) {
    for (int i = 0; i < size; i++) {
        if (Sframe_already_defined(frame, f_global[i]->address)) {
            __ERROR("Error frame.c: global already defined\n");
        }
        
        frame->f_globals[frame->f_globals_index++] = f_global[i];
    }

    frame->f_globals_size += size;
    return frame;
}

struct Sframe *
Sframe_insert_local
(struct Sframe *frame, struct Sobj **f_local, int size) {
    for (int i = 0; i < size; i++) {
        if (Sframe_already_defined(frame, f_local[i]->address)) {
            __ERROR("Error frame.c: local already defined\n");
        }

        frame->f_locals[frame->f_locals_index++] = f_local[i];
    }

    frame->f_locals_size += size;

    return frame;
}

int
Sframe_already_defined
(struct Sframe *frame, int address) {
    for (int i = 0; i < frame->f_locals_size; i++) {
        if (frame->f_locals[i]->address == address) {
            return 1;
        }
    }

    return 0;
}

struct Sobj *
Sframe_load_c_api_func
(struct Sframe *frame, void* func, int address, char* name, int args_size) {
    struct Sc_api_func* api_func = Sc_api_func_set(func, name, address, args_size);

    struct Sobj *load = Sobj_new();

    load->type = BUILTIN_OBJ;
    load->c_api_func = api_func;

    Sframe_store_global(frame, address, load, GLOBAL_OBJ);

    return load;
}

void*
Sframe_find_c_api_func
(struct Sframe *frame, int address) {
    for (int i = 0; i < frame->f_globals_size; i++) {
        if (frame->f_globals[i]->address == address) {
            return frame->f_globals[i]->c_api_func->func;
        }
    }

    __ERROR("Error: function not found\n");
    return NULL;
}

struct Sframe*
Sframe_push_number(struct Sframe* frame, double number) {
    struct Sobj* obj = Svalue(number);
    Sframe_push(frame, obj);
    return frame;
}

struct Sframe*
Sframe_push_string(struct Sframe* frame, char* string, int size) {
    struct Sobj* obj = Sobj_make_string(string, size);
    Sframe_push(frame, obj);
    return frame;
}

struct Sframe*
Sframe_push_bool(struct Sframe* frame, int b) {
    struct Sobj* obj = Sobj_make_bool(b);
    Sframe_push(frame, obj);
    return frame;
}

struct Sobj*
Sframe_get_top(struct Sframe* frame) {
    if (frame->f_stack_index <= 0) {
        __ERROR("Error frame.c: stack underflow\n");
        return NULL;
    }

    struct Sobj* obj = frame->f_stack[frame->f_stack_index - 1];

    if (obj) {
        return obj;
    } else {
        __ERROR("Error frame.c: stack underflow\n");
        return NULL;
    }
}

struct Sframe*
Sframe_call_c_api_func(struct Sframe* frame, void* func) {
    if (!func) {
        __ERROR("Error frame.c: func is null\n");
        return NULL;
    }

    struct Sobj* (*f)(struct Sframe*) = (struct Sobj* (*)(struct Sframe*)) func;
    struct Sobj* obj = f(frame);

    if (!obj) {
        __ERROR("Error frame.c: obj is null\n");
        return NULL;
    }

    Sframe_push(frame, obj);
    return frame;
}

struct Sobj*
Sframe_true_pop(struct Sframe* frame) {
    struct Sobj* obj = Sframe_pop(frame);
    MOVETOGC(obj, frame->gc_pool);
    return obj;
}

struct Sframe*
Sframe_push_null(struct Sframe* frame) {
    struct Sobj* obj = Sobj_make_null();
    Sframe_push(frame, obj);
    return frame;
}

struct Sobj*
Sframe_store_closure
(struct Sframe *frame, int address, struct Sobj *obj, enum Sobj_t type) {
    obj->is_closure = 1;
    _SUNYINCREF(obj);

    for (int i = 0; i < frame->f_locals_size; i++) {
        if (frame->f_locals[i]->address == address && frame->f_locals[i]->is_closure) {
            struct Sobj *old = frame->f_locals[i]->f_value;

            _SUNYDECREF(old);
            MOVETOGC(old, frame->gc_pool);

            frame->f_locals[i]->f_value = obj;
            frame->f_locals[i]->type = type;
            frame->f_locals[i]->address = address;

            return frame->f_locals[i];
        }
    }

    for (int i = 0; i < frame->f_globals_size; i++) {
        if (frame->f_globals[i]->address == address) {
            struct Sobj *old = frame->f_globals[i]->f_value;

            _SUNYDECREF(old);
            MOVETOGC(old, frame->gc_pool);

            frame->f_globals[i]->f_value = obj;
            frame->f_globals[i]->type = type;
            frame->f_globals[i]->address = address;

            return frame->f_globals[i];
        }
    }

    struct Sobj *local = Sobj_new();

    local->is_closure = 1;

    local->type = LOCAL_OBJ;
    local->f_value = obj;
    local->f_value->address = address;
    local->address = address;

    frame->f_locals[frame->f_locals_index++] = local;
    frame->f_locals_size++;

    return local;
}

struct Sobj *
Sframe_load_closure
(struct Sframe *frame, int address) {
    struct Sobj *load = NULL;

    for (int i = 0; i < frame->f_locals_size; i++) {
        if (frame->f_locals[i]->address == address && frame->f_locals[i]->is_closure) {
            load = frame->f_locals[i];
            break;
        }
    }

    if (!load) {
        for (int i = 0; i < frame->f_globals_size; i++) {
            if (frame->f_globals[i]->address == address) {
                load = frame->f_globals[i];
                break;
            }
        }
    }

    if (!load) {
        __ERROR("Error frame.c: global not found address: %d\n", address);
        return NULL;
    }

    return load;   
}
