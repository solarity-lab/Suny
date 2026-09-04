#ifndef FILELIB_H
#define FILELIB_H

#include "./File/sfile.h"

#include "../../../Suny/Suny/Suny.h"

SUNY_API struct Sobj* Sobj_make_sfile(struct SFile* file);

SUNY_API struct Sobj* Sio_print_sfile(struct Sobj* obj);

SUNY_API int Sobj_free_sfile(struct Sobj* obj);

#endif // FILELIB_H