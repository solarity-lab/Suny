#include "stype.h"
#include "smem.h"

struct Stype *
Stype_new(void) {
    struct Stype *type = Smem_Malloc(sizeof(struct Stype));
    return type;
}

int 
Stype_free
(struct Stype *type) {
    Smem_Free(type);
    return 0;
}