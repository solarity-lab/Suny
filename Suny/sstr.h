#ifndef SSTR_H
#define SSTR_H

#include <string.h>

#include "sobj.h"
#include "sio.h"

struct Sstr *Sstr_new(void);

struct Sstr *Sstr_cast_to_string(struct Sobj *obj);

struct Sstr *Sstr_new_from_char(char *chr, int size);

void Sstr_free(struct Sstr *str);

struct Sstr *Sstr_add(struct Sstr *str1, struct Sstr *str2);

struct Sstr *Sstr_mod(struct Sstr *str1, struct Sstr *str2);

struct Sstr *Sstr_mul(struct Sstr *str, int n);

struct Sobj* Sobj_make_string_object(struct Sstr *str);

struct Sobj *Sobj_make_string(char* str, int size);

struct Sobj *Sobj_make_char(char chr);

int Sstr_cmp(struct Sstr *str1, struct Sstr *str2);

#endif