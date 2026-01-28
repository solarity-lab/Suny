#include "sframe.h"
#include "sdebug.h"
#include "smem.h"

struct Sframe *
Sframe_new(void) {
    SDEBUG("[sframe.c] Sframe_new(void)\n");
    struct Sframe *frame = Smem_Malloc(sizeof(struct Sframe));

    frame->f_code = NULL;

    frame->f_globals_index = 0;
    frame->f_globals_size = 0;

    frame->f_locals_index = 0;
    frame->f_locals_size = 0;

    frame->f_heap_index = 0;
    frame->f_heap_size = DEFAULT_MAX;

    frame->f_stack_index = 0;
    frame->f_stack_size = DEFAULT_MAX;

    frame->f_const_index = 0;
    frame->f_const_size = DEFAULT_MAX;

    frame->f_stack = Smem_Calloc(DEFAULT_MAX, sizeof(struct Sobj *));
    frame->f_locals = Smem_Calloc(DEFAULT_MAX, sizeof(struct Sobj *));
    frame->f_globals = Smem_Calloc(DEFAULT_MAX, sizeof(struct Sobj *));
    frame->f_consts = Smem_Calloc(DEFAULT_MAX, sizeof(struct Sobj *));
    
    frame->f_heaps = NULL;

    frame->f_label_map = NULL;
    frame->gc_pool = NULL;
    frame->compiler = NULL;
    frame->table = NULL;

    frame->f_code_index = 0;

    frame->f_obj = NULL;

    frame->GC_THRESHOLD = POOL_SIZE_LIMIT;

    SDEBUG("[sframe.c] Sframe_new(void) (done)\n");
    return frame;
}

int
Sframe_free
(struct Sframe *frame) {
    SDEBUG("[sframe.c] Sframe_free(struct Sframe *frame)\n");
    
    Sobj_free_objs(frame->f_stack, frame->f_stack_index);
    Sobj_free_objs(frame->f_locals, frame->f_locals_index);
    Sobj_free_objs(frame->f_globals, frame->f_globals_index);
    Sobj_free_objs(frame->f_consts, frame->f_const_index);

    if (frame->f_label_map) {
        Smem_Free(frame->f_label_map);
    }

    Sgc_free_pool(frame->gc_pool);

    Smem_Free(frame);
    SDEBUG("[sframe.c] Sframe_free(struct Sframe *frame) (done)\n");
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
    SDEBUG("[sframe.c] Sframe_push(struct Sframe *frame, struct Sobj *obj)\n");

    if (frame->f_stack_index >= frame->f_stack_size) {
        frame->f_stack_size *= 2;
        frame->f_stack = Smem_Realloc(frame->f_stack, frame->f_stack_size * sizeof(struct Sobj *));
    }

    _SUNYINCREF(obj);

    frame->f_stack[frame->f_stack_index++] = obj;
    SDEBUG("[sframe.c] Sframe_push(struct Sframe *frame, struct Sobj *obj) (done)\n");
    return obj;
}

struct Sobj *
Sframe_pop
(struct Sframe *frame) {
    SDEBUG("[sframe.c] Sframe_pop(struct Sframe *frame) (building...)\n");
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
    SDEBUG("[sframe.c] Sframe_pop(struct Sframe *frame) (done)\n");
    return obj;
}

struct Sobj *
Sframe_back
(struct Sframe *frame) {
    SDEBUG("[sframe.c] Sframe_back(struct Sframe *frame)\n");
    if (frame->f_stack_index <= 0) {
        return NULL;
    }

    struct Sobj *obj = frame->f_stack[frame->f_stack_index - 1];

    if (!obj) {
        SDEBUG("[sframe.c] Sframe_back(struct Sframe *frame) (done)\n");
        return NULL;
    }

    SDEBUG("[sframe.c] Sframe_back(struct Sframe *frame) (done)\n");
    return obj;
}

struct Sobj*
Sframe_store_global
(struct Sframe *frame, int address, struct Sobj *obj, enum Sobj_t type) {
    SDEBUG("[sframe.c] Sframe_store_global(...)\n");

    for (int i = 0; i < frame->f_globals_size; i++) {
        if (frame->f_globals[i]->address == address) {
            struct Sobj *old = frame->f_globals[i]->f_value;

            _SUNYDECREF(old);
            MOVETOGC(old, frame->gc_pool);

            _SUNYINCREF(obj);

            frame->f_globals[i]->f_value = obj;
            frame->f_globals[i]->type = type;
            frame->f_globals[i]->address = address;

            SDEBUG("[sframe.c] Sframe_store_global(done, overwrite)\n");
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

    SDEBUG("[sframe.c] Sframe_store_global(done, new)\n");
    return global;
}

struct Sobj *
Sframe_load_global
(struct Sframe *frame, int address) {
    SDEBUG("[sframe.c] Sframe_load_global(struct Sframe *frame, int address)\n");
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
    SDEBUG("[sframe.c] Sframe_store_local(struct Sframe *frame, int address, struct Sobj *obj, enum Sobj_t type)\n");

    for (int i = 0; i < frame->f_locals_size; i++) {
        if (frame->f_locals[i]->address == address && !frame->f_locals[i]->is_closure) {
            struct Sobj *old = frame->f_locals[i]->f_value;

            _SUNYDECREF(old);
            MOVETOGC(old, frame->gc_pool);

            _SUNYINCREF(obj);

            frame->f_locals[i]->f_value = obj;
            frame->f_locals[i]->type = type;
            frame->f_locals[i]->address = address;

            SDEBUG("[sframe.c] Sframe_store_local(struct Sframe *frame, int address, struct Sobj *obj, enum Sobj_t type) (done)\n");
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
            frame->f_globals[i]->address = address;

            SDEBUG("[sframe.c] Sframe_store_local(struct Sframe *frame, int address, struct Sobj *obj, enum Sobj_t type) (done)\n");
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

    SDEBUG("[sframe.c] Sframe_store_local(struct Sframe *frame, int address, struct Sobj *obj, enum Sobj_t type) (done)\n");
    return local;
}

struct Sobj *
Sframe_load_local
(struct Sframe *frame, int address) {
    SDEBUG("[sframe.c] Sframe_load_local(struct Sframe *frame, int address)\n");
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

    SDEBUG("[sframe.c] Sframe_load_local(struct Sframe *frame, int address) (done)\n");
    return load;
}

struct Sframe *
Sframe_insert_global
(struct Sframe *frame, struct Sobj **f_global, int size) {
    SDEBUG("[sframe.c] Sframe_insert_global(struct Sframe *frame, struct Sobj **f_global, int size)\n");
    for (int i = 0; i < size; i++) {
        if (Sframe_already_defined(frame, f_global[i]->address)) {
            __ERROR("Error frame.c: global already defined\n");
        }
        
        frame->f_globals[frame->f_globals_index++] = f_global[i];
    }

    frame->f_globals_size += size;
    SDEBUG("[sframe.c] Sframe_insert_global(struct Sframe *frame, struct Sobj **f_global, int size) (done)\n");
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
    SDEBUG("[sframe.c] Sframe_push_number(struct Sframe* frame, float number)\n");
    struct Sobj* obj = Svalue(number);
    Sframe_push(frame, obj);
    SDEBUG("[sframe.c] Sframe_push_number(struct Sframe* frame, float number) (done)\n");
    return frame;
}

struct Sframe*
Sframe_push_string(struct Sframe* frame, char* string, int size) {
    SDEBUG("[sframe.c] Sframe_push_string(struct Sframe* frame, char* string, int size)\n");
    struct Sobj* obj = Sobj_make_string(string, size);
    Sframe_push(frame, obj);
    SDEBUG("[sframe.c] Sframe_push_string(struct Sframe* frame, char* string, int size) (done)\n");
    return frame;
}

struct Sframe*
Sframe_push_bool(struct Sframe* frame, int b) {
    SDEBUG("[sframe.c] Sframe_push_bool(struct Sframe* frame, int b)\n");
    struct Sobj* obj = Sobj_make_bool(b);
    Sframe_push(frame, obj);
    SDEBUG("[sframe.c] Sframe_push_bool(struct Sframe* frame, int b) (done)\n");
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
    SDEBUG("[sframe.c] Sframe_push_null(struct Sframe* frame)\n");
    struct Sobj* obj = Sobj_make_null();
    Sframe_push(frame, obj);
    SDEBUG("[sframe.c] Sframe_push_null(struct Sframe* frame) (done)\n");
    return frame;
}

struct Sobj *
Sframe_load_const
(struct Sframe *frame, int index) {
    if (index < 0 || index >= frame->f_const_size) {
        __ERROR("Error frame.c: const index out of range: %d\n", index);
        return NULL;
    }

    return frame->f_consts[index];
}

struct Sobj *
Sframe_store_const
(struct Sframe *frame, int index, struct Sobj *obj) {
    _SUNYINCREF(obj);

    if (index < 0) {
        __ERROR("Error frame.c: const index out of range: %d\n", index);
        return NULL;
    }

    if (index >= frame->f_const_size) {
        frame->f_const_size *= 2;
        frame->f_consts = Smem_Realloc(frame->f_consts, frame->f_const_size * sizeof(struct Sobj *));
    }

    if (frame->f_consts[index]) {
        struct Sobj *old = frame->f_consts[index];
        _SUNYDECREF(old);
        MOVETOGC(old, frame->gc_pool);
    }

    frame->f_consts[index] = obj;

    return obj;
}

struct Sobj*
Sframe_store_closure
(struct Sframe *frame, int address, struct Sobj *obj, enum Sobj_t type) {
    SDEBUG("[sframe.c] Sframe_store_closure(struct Sframe *frame, int address, struct Sobj *obj, enum Sobj_t type)\n");
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

            SDEBUG("[sframe.c] Sframe_store_closure(struct Sframe *frame, int address, struct Sobj *obj, enum Sobj_t type) (done)\n");
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

            SDEBUG("[sframe.c] Sframe_store_global(struct Sframe *frame, int address, struct Sobj *obj, enum Sobj_t type) (done)\n");
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

    SDEBUG("[sframe.c] Sframe_store_closure(struct Sframe *frame, int address, struct Sobj *obj, enum Sobj_t type) (done)\n");
    return local;
}

struct Sobj *
Sframe_load_closure
(struct Sframe *frame, int address) {
    SDEBUG("[sframe.c] Sframe_load_closure(struct Sframe *frame, int address)\n");
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

    SDEBUG("[sframe.c] Sframe_load_closure(struct Sframe *frame, int address) (done)\n");
    return load;   
}