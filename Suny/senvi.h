#ifndef SENVI_H
#define SENVI_H

#include "score.h"
#include "sobj.h"
#include "sframe.h"
#include "scopy.h"

struct Senvi {
    struct Sobj** envi;
    int size;
    int capacity;
};

struct Senvi* Senvi_new(void);
struct Senvi* Senvi_copy(struct Senvi* envi);
int Senvi_free(struct Senvi* envi);

int Senvi_set_local(struct Sframe* frame, struct Senvi* envi);
int Senvi_set_global(struct Sframe* frame, struct Senvi* envi);
int Senvi_push(struct Senvi* envi, struct Sobj* obj);
int Senvi_pop(struct Senvi* envi);

int Senvi_store(struct Senvi* envi, struct Sobj* var);

struct Sobj* Senvi_get(struct Senvi* envi, int address);

#endif // SENVI_H