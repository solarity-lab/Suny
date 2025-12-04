#ifndef SMETA_H
#define SMETA_H

#include "sobj.h"

struct Smeta {
    struct Sobj* (*mm_add)(struct Sobj*, struct Sobj*); // +
    struct Sobj* (*mm_sub)(struct Sobj*, struct Sobj*); // -
    struct Sobj* (*mm_mul)(struct Sobj*, struct Sobj*); // *
    struct Sobj* (*mm_div)(struct Sobj*, struct Sobj*); // /

    struct Sobj* (*mm_eq)(struct Sobj*, struct Sobj*); // ==
    struct Sobj* (*mm_ne)(struct Sobj*, struct Sobj*); // !=
    struct Sobj* (*mm_gt)(struct Sobj*, struct Sobj*); // >
    struct Sobj* (*mm_ge)(struct Sobj*, struct Sobj*); // >=
    struct Sobj* (*mm_lt)(struct Sobj*, struct Sobj*); // <
    struct Sobj* (*mm_le)(struct Sobj*, struct Sobj*); // <=

    struct Sobj* (*mm_call)(struct Sobj*, struct Sobj*); // ()
    struct Sobj* (*mm_index)(struct Sobj*, struct Sobj*); // []

    struct Sobj* (*mm_tostring)(struct Sobj*); // to string
    struct Sobj* (*mm_type)(struct Sobj*); // type
    
    int (*mm_store_item)(struct Sobj*, struct Sobj*, struct Sobj*);
    int (*mm_free)(struct Sobj*); // Smem_Free

    struct Smeta* prev;
    struct Sobj* meta_f_add;
    struct Sobj* meta_f_sub;
    struct Sobj* meta_f_mul;
    struct Sobj* meta_f_div;
    struct Sobj* meta_f_eq;
    struct Sobj* meta_f_ne;
    struct Sobj* meta_f_gt;
    struct Sobj* meta_f_ge;
    struct Sobj* meta_f_lt;
    struct Sobj* meta_f_le;
    struct Sobj* meta_f_call;
    struct Sobj* meta_f_index;
    struct Sobj* meta_f_tostring;
    struct Sobj* meta_f_type;

    int is_meta_class;

    struct Sframe* frame;
};

struct Smeta* Smeta_new(void);

void Smeta_set(struct Sobj* obj, const char* name, void* fn);

struct Smeta* Sobj_set_meta(struct Sobj* obj, struct Smeta* meta);

void* Sobj_get_metamethod(struct Sobj* obj, const char* name);

#endif // SMETA_H