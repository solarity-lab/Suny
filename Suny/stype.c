#include "stype.h"
#include "smem.h"

struct Stype *
Stype_new(void) {
    struct Stype *type = Smem_Malloc(sizeof(struct Stype));
    type->f_call_context = NULL;
    type->f_code = NULL;
    type->f_frame = NULL;
    type->f_func = NULL;
    type->f_str = NULL;
    type->f_obj = NULL;
    type->f_bool = NULL;
    type->f_list = NULL;
    return type;
}

int 
Stype_free
(struct Stype *type) {
    Smem_Free(type);
    return 0;
}