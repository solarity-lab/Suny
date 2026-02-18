#include "suserdata.h"
#include "smem.h"

struct Suserdata* Suserdata_new(void) {
    struct Suserdata* userdata = Smem_Malloc(sizeof(struct Suserdata));
    return userdata;
}
struct Suserdata* Suserdata_set(void* data) {
    struct Suserdata* userdata = Suserdata_new();
    userdata->data = data;
    return userdata;
}

struct Sobj* Sobj_make_userdata(void* data) {
    struct Suserdata* userdata = Suserdata_set(data);
    struct Sobj* obj = Sobj_new();
    obj->type = USER_DATA_OBJ;
    obj->f_type = Stype_new();
    obj->f_type->f_userdata = userdata;
    return obj;
}

int Suserdata_free(struct Suserdata* userdata) {
    Smem_Free(userdata);
    return 0;
}