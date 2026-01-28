#include "scopy.h"

struct Sobj* Sobj_shallow_copy(struct Sobj* obj) {
    struct Sobj* new_obj = Sobj_new();
    new_obj->type = obj->type;
    new_obj->f_type = obj->f_type;
    new_obj->address = obj->address;

    new_obj->is_argument = obj->is_argument;
    new_obj->is_global = obj->is_global;
    new_obj->is_local = obj->is_local;
    new_obj->is_closure = obj->is_closure;
    new_obj->is_return = obj->is_return;
    new_obj->is_calle = obj->is_calle;
    new_obj->is_belong_class = obj->is_belong_class;

    new_obj->f_value = obj->f_value;
    return new_obj;
}

struct Sobj* Sobj_deep_copy(struct Sobj* obj) {
    struct Sobj* new_obj = Sobj_shallow_copy(obj);
    
    if (obj->type == STRING_OBJ) {
        new_obj->f_type = Stype_new();
        new_obj->f_type->f_str = Sstr_new_from_char(obj->f_type->f_str->string, obj->f_type->f_str->size);
        new_obj->f_type->f_str->size = obj->f_type->f_str->size;
    } else if (obj->type == LIST_OBJ) {
        new_obj->f_type = Stype_new();
        struct Slist* new_list = Slist_deep_copy(obj->f_type->f_list);
        new_obj->f_type->f_list = new_list;
    } else if (obj->type == CLASS_OBJ) {
        new_obj->f_type = Stype_new();
        struct Sclass* new_sclass = Sclass_deep_copy(obj->f_type->f_class);
        new_obj->f_type->f_class = new_sclass;
    } else if (obj->type == FUNC_OBJ) {
        new_obj->f_type = Stype_new();
        struct Sfunc* new_func = Sfunc_deep_copy(obj->f_type->f_func);
        new_obj->f_type->f_func = new_func;
    } else if (obj->type == CLOSURE_OBJ) {
        new_obj->f_type = Stype_new();
        struct Sfunc* new_func = Sclosure_deep_copy(obj->f_type->f_func);
        new_obj->f_type->f_func = new_func;
    }

    return new_obj;
}

struct Scode* Scode_deep_copy(struct Scode* code) {
    struct Scode* new_code = Scode_new();
    for (int i = 0; i < code->size; i++) {
        Scode_add(new_code, code->code[i]);
    }
    return new_code;
}

struct Sfunc* Sfunc_deep_copy(struct Sfunc* func) {
    struct Sfunc* new_func = Sfunc_new();
    new_func->args_size = func->args_size;
    new_func->code_size = func->code_size;
    new_func->code = Scode_deep_copy(func->code);
    new_func->args_index = func->args_index;

    for (int i = 0; i < func->args_size; i++) {
        new_func->args_address[i] = func->args_address[i];
    }
    return new_func;
}

struct Slist* Slist_deep_copy(struct Slist* list) {
    struct Slist* new_list = Slist_new();
    for (int i = 0; i < list->count; i++) {
        Slist_add(new_list, Sobj_deep_copy(list->array[i]));
    }
    return new_list;
}

struct Sclass* Sclass_deep_copy(struct Sclass* sclass) {
    struct Sclass* new_sclass = Sclass_new();

    for (int i = 0; i < sclass->count; i++) {
        struct Sobj* new_obj = Sobj_deep_copy(sclass->methods[i]->f_value);

        struct Sobj* method = Sobj_new();
        method->type = LOCAL_OBJ;
        method->f_value = new_obj; 
        method->address = sclass->methods[i]->address;

        _SUNYINCREF(new_obj);

        Sclass_push_obj(new_sclass, method);
    }

    return new_sclass;
}

struct Sobj* Sobj_copy_variable(struct Sobj* obj) {
    struct Sobj* scope = Sobj_shallow_copy(obj);
    scope->f_value = Sobj_deep_copy(obj->f_value);
    return scope;
}

struct Sfunc* Sclosure_deep_copy(struct Sfunc* closure) {
    struct Sfunc* new_func = Sfunc_deep_copy(closure);
    new_func->is_closure = 1;
    new_func->envi = Senvi_copy(closure->envi);
    return new_func;
}

struct Sobj* Sobj_copy_wrapper(struct Sobj* obj) {
    if (obj->type != LOCAL_OBJ && obj->type != GLOBAL_OBJ) return obj;
    struct Sobj* scope = Sobj_shallow_copy(obj);
    scope->f_value = obj->f_value;
    return scope;
}