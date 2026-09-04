#include "FileLib.h"

struct Sobj* Sobj_make_sfile(struct SFile* file) {
    struct Sobj* obj = Sobj_make_userdata(file);
    
    obj->meta = Smeta_new();
    obj->meta->mm_tostring = Sio_print_sfile;
    obj->meta->mm_free = Sobj_free_sfile;

    return obj;
}

struct Sobj* Sio_print_sfile(struct Sobj* obj) {
    struct SFile* file = get_userdata(obj);
    sfile_print(file);
    return obj;
}

int Sobj_free_sfile(struct Sobj* obj) {
    struct SFile* file = get_userdata(obj);
    sfile_close(file);
    return 0;
}