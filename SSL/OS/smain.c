#include "FileLib.h"

SUNY_API struct Sobj* Sfopen(struct Sframe* frame) {
    struct Sobj* mode_obj = Sframe_pop(frame);
    struct Sobj* file_name_obj = Sframe_pop(frame);

    char* file_name = tget_str(file_name_obj)->string;
    char* mode = tget_str(mode_obj)->string;

    struct SFile* file = sfile_open(file_name, mode);
    return Sobj_make_sfile(file);
}

SUNY_API struct Sobj* Sfclose(struct Sframe* frame) {
    struct Sobj* file_obj = Sframe_pop(frame);
    struct SFile* file = get_userdata(file_obj);
    sfile_close(file);
    return null_obj;
}

SUNY_API struct Sobj* Sfprint(struct Sframe* frame) {
    struct Sobj* file_obj = Sframe_pop(frame);
    struct SFile* file = get_userdata(file_obj);
    sfile_print(file);
    return null_obj;
}

SUNY_API struct Sobj* Sfwrite(struct Sframe* frame) {
    struct Sobj* len_obj = Sframe_pop(frame);
    struct Sobj* data_obj = Sframe_pop(frame);
    struct Sobj* file_obj = Sframe_pop(frame);

    struct SFile* file = get_userdata(file_obj);

    char* data = tget_str(data_obj)->string;
    size_t len = (size_t) ValueOf(len_obj);
    sfile_write(file, data, len);

    return null_obj;
}

SUNY_API struct Sobj* Sfdelete(struct Sframe* frame) {
    struct Sobj* file_obj = Sframe_pop(frame);

    struct SFile* file = get_userdata(file_obj);

    sfile_delete(file);

    return null_obj;
}

SUNY_API struct Sobj* Sfclear(struct Sframe* frame) {
    struct Sobj* file_obj = Sframe_pop(frame);

    struct SFile* file = get_userdata(file_obj);

    sfile_clear(file);

    return null_obj;
}

SUNY_API struct Sframe* Smain(struct Sframe* frame, struct Stable *table) {
    Sinitialize_c_api_func(frame, table, 20, 2, "fopen", Sfopen);
    Sinitialize_c_api_func(frame, table, 21, 1, "fclose", Sfclose);
    Sinitialize_c_api_func(frame, table, 22, 1, "fprint", Sfprint);
    Sinitialize_c_api_func(frame, table, 23, 3, "fwrite", Sfwrite);
    Sinitialize_c_api_func(frame, table, 24, 1, "fdelete", Sfdelete);
    Sinitialize_c_api_func(frame, table, 25, 1, "fclear", Sfclear);
    return frame;
}