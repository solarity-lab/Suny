#include "senvi.h"
#include "sdebug.h"
#include "smem.h"

struct Senvi* Senvi_new(void) {
    struct Senvi* envi = Smem_Malloc(sizeof(struct Senvi));
    envi->size = 0;
    envi->capacity = 1024;
    envi->envi = Smem_Malloc(sizeof(struct Sobj*) * envi->capacity);
    return envi;
}

int Senvi_free(struct Senvi* envi) {
    for (int i = 0; i < envi->size; i++) {
        Sobj_free(envi->envi[i]->f_value);
        Sobj_free(envi->envi[i]);    
    }

    Smem_Free(envi->envi);
    Smem_Free(envi);
    return 0;
}

int Senvi_set_local(struct Sframe* frame, struct Senvi* envi) {
    for (int i = 0; i < frame->f_locals_size; ++i) {
        if (frame->f_locals[i] == NULL) continue;
        if (frame->f_locals[i]->type != LOCAL_OBJ) {
            __ERROR("Not an local object\n");
        }

        envi->envi[envi->size++] = frame->f_locals[i];
    }

    return 0;
}

int Senvi_set_global(struct Sframe* frame, struct Senvi* envi) {
    for (int i = 0; i < frame->f_globals_size; ++i) {
        Senvi_push(envi, frame->f_globals[i]);
    }
    return 0;
}

int Senvi_push(struct Senvi* envi, struct Sobj* obj) {
    if (envi->size >= envi->capacity) {
        envi->capacity *= 2;
        envi->envi = Smem_Realloc(envi->envi, sizeof(struct Sobj*) * envi->capacity);
    }
    envi->envi[envi->size++] = obj;

    return 0;
}

int Senvi_pop(struct Senvi* envi) {
    envi->envi[envi->size - 1] = NULL;
    envi->size -= 1;
    return 0;
}

struct Sobj* Senvi_get(struct Senvi* envi, int address) {
    for (int i = 0; i < envi->size; ++i) {
        if (envi->envi[i]->address == address) {
            return envi->envi[i];
        }
    }
 
    return NULL;
}

int Senvi_store(struct Senvi* envi, struct Sobj* var) {
    for (int i = 0; i < envi->size; ++i) {
        if (envi->envi[i]->address == var->address) {
            return 0;
        }
    }

    var->f_value->is_closure = 1;
    var->is_closure = 1;
    Senvi_push(envi, var);
    return 0;
}

struct Senvi* Senvi_copy(struct Senvi* envi) {
    struct Senvi* new_envi = Senvi_new();
    for (int i = 0; i < envi->size; ++i) {
        struct Sobj* scope = envi->envi[i];
        struct Sobj* new_scope = Sobj_copy_variable(scope);
        Senvi_push(new_envi, new_scope);
    }

    return new_envi;
}