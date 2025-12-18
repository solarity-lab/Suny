#include "sclass.h"
#include "smem.h"

struct Sclass*
Sclass_new
(void) {
    struct Sclass* sclass = Smem_Malloc(sizeof(struct Sclass));

    sclass->count = 0;
    sclass->capacity = 1024;
    sclass->methods = Smem_Malloc(sizeof(struct Sobj*) * sclass->capacity);

    return sclass;
}

int 
Sclass_free
(struct Sclass* sclass) {
    Smem_Free(sclass->methods);
    for (int i = 0; i < sclass->count; i++) {
        Sobj_free(sclass->methods[i]);
    }
    Smem_Free(sclass);
    return 0;
}

struct Sclass* 
Sclass_store_object
(struct Sclass* sclass, struct Sframe* frame, int address) {
#ifdef DEBUG
    printf("[sclass.c] struct Sclass* Sclass_store_object(struct Sclass* sclass, struct Sframe* frame, int address) (building...)\n");
#endif
    struct Sobj* object = Sframe_pop(frame);

    if (sclass->count == sclass->capacity) {
        sclass->capacity *= 2;
        sclass->methods = Smem_Realloc(sclass->methods, sizeof(struct Sobj*) * sclass->capacity);
    }

    for (int i = 0; i < sclass->count; i++) {
        if (sclass->methods[i]->address == address) {
            _SUNYDECREF(sclass->methods[i]->f_value);
            MOVETOGC(sclass->methods[i]->f_value, frame->gc_pool);
            sclass->methods[i]->f_value = object;
            return sclass;
        }
    }

    struct Sobj* method = Sobj_new();
    method->type = LOCAL_OBJ;
    method->f_value = object;
    method->address = address;

    _SUNYINCREF(object);
    _SUNYINCREF(method);

    sclass->methods[sclass->count++] = method;

#ifdef DEBUG
    printf("[sclass.c] struct Sclass* Sclass_store_object(struct Sclass* sclass, struct Sframe* frame, int address) (done)\n");
#endif

    return sclass;
}

struct Sobj* 
Sclass_get_object
(struct Sclass* sclass, int address) {
    for (int i = 0; i < sclass->count; i++) {
        if (sclass->methods[i]->address == address) {
            return sclass->methods[i];
        }
    }

    return NULL;
}

struct Sobj*
Sobj_make_class(struct Sclass* sclass) {
    struct Sobj* obj = Sobj_new();
    obj->type = CLASS_OBJ;
    obj->f_type = Stype_new();
    obj->meta = Smeta_new();
    obj->f_type->f_class = sclass;
    return obj;
}

struct Sobj* 
Sclass_push_obj
(struct Sclass* sclass, struct Sobj* obj) {
    if (sclass->count == sclass->capacity) {
        sclass->capacity *= 2;
        sclass->methods = Smem_Realloc(sclass->methods, sizeof(struct Sobj*) * sclass->capacity);
    }

    _SUNYINCREF(obj->f_value);
    sclass->methods[sclass->count++] = obj;
    return obj;
}

struct Sclass* 
Sclass_store_local_obj
(struct Sclass* sclass, struct Sframe* frame, struct Sobj* value, int address) {
    for (int i = 0; i < sclass->count; i++) {
        if (sclass->methods[i]->address == address) {
            _SUNYDECREF(sclass->methods[i]->f_value);
            MOVETOGC(sclass->methods[i]->f_value, frame->gc_pool);

            sclass->methods[i]->f_value = value;
            return sclass;
        }
    }

    struct Sobj* method = Sobj_new();
    method->type = LOCAL_OBJ;
    method->f_value = value;
    method->address = address;

    _SUNYINCREF(value);
    _SUNYINCREF(method);

    sclass->methods[sclass->count++] = method;
    
    return NULL;
}

struct Sobj*
Sobj_copy_class(struct Sclass* sclass) {
    struct Sclass* new_sclass = Sclass_copy(sclass);
    return Sobj_make_class(new_sclass);
}   

int
Sclass_has_method
(struct Sclass* sclass, int address) {
    for (int i = 0; i < sclass->count; i++) {
        if (sclass->methods[i]->address == address) {
            return 1;
        }
    }

    if (sclass->super_class) {
        return Sclass_has_method(sclass->super_class, address);
    }

    return 0;
}

struct Sclass* 
Sclass_extends_class
(struct Sclass* sclass, struct Sclass* super_class) {
    sclass->super_class = super_class;
    for (int i = 0; i < super_class->count; i++) {
        Sclass_push_obj(sclass, super_class->methods[i]);
    }
    return sclass;
}

struct Sframe*
Sclass_call(struct Sframe* frame, struct Sobj* f_obj) {
    if (f_obj->type != CLASS_OBJ) {
        return frame;
    }

    struct Sobj *obj = Sobj_copy_class(f_obj->f_type->f_class);

    obj->prev = f_obj;

    struct Sobj *init_f = Sclass_get_object(obj->f_type->f_class, __INIT__ADDRESS);
    
    struct Sobj* add_f = Sclass_get_object(obj->f_type->f_class, __ADD__ADDRESS);
    struct Sobj* sub_f = Sclass_get_object(obj->f_type->f_class, __SUB__ADDRESS);
    struct Sobj* mul_f = Sclass_get_object(obj->f_type->f_class, __MUL__ADDRESS);
    struct Sobj* div_f = Sclass_get_object(obj->f_type->f_class, __DIV__ADDRESS);

    struct Sobj* tostring_f = Sclass_get_object(obj->f_type->f_class, __TO_STR__ADDRESS);

    if (add_f && add_f->f_value->type == FUNC_OBJ) {
        obj->meta->meta_f_add = add_f->f_value;
        add_f->f_value->prev = obj;
        obj->meta->is_meta_class = 1;
    }

    if (sub_f && sub_f->f_value->type == FUNC_OBJ) {
        obj->meta->meta_f_sub = sub_f->f_value;
        sub_f->f_value->prev = obj;
        obj->meta->is_meta_class = 1;
    }

    if (mul_f && mul_f->f_value->type == FUNC_OBJ) {
        obj->meta->meta_f_mul = mul_f->f_value;
        mul_f->f_value->prev = obj;
        obj->meta->is_meta_class = 1;
    }

    if (div_f && div_f->f_value->type == FUNC_OBJ) {
        obj->meta->meta_f_div = div_f->f_value;
        div_f->f_value->prev = obj;
        obj->meta->is_meta_class = 1;
    }

    if (tostring_f && tostring_f->f_value->type == FUNC_OBJ) {
        obj->meta->meta_f_tostring = tostring_f->f_value;
        tostring_f->f_value->prev = obj;
        obj->meta->is_meta_class = 1;
    }

    if (init_f) {
        struct Scall_context *context = Scall_context_new();

        init_f->f_value->prev = obj;

        Scall_context_set_real_func(context, frame, init_f->f_value);
        Svm_run_call_context(context);
        Scall_context_free(context);
    }

    Sframe_push(frame, obj);

    return frame;
}