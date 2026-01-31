#include "sstr.h"
#include "smem.h"

struct Sstr *Sstr_new(void) {
    struct Sstr *str = Smem_Malloc(sizeof(struct Sstr));
    str->size = 0;
    str->string = NULL;
    return str;
}

struct Sstr *Sstr_new_from_char(char *chr, int size) {
    struct Sstr *str = Smem_Malloc(sizeof(struct Sstr));

    str->size = size;

    str->string = Smem_Malloc(size + 1);
    memcpy(str->string, chr, size);

    str->string[size] = '\0';

    return str;
}

void Sstr_free(struct Sstr *str) {
    Smem_Free(str->string);
    Smem_Free(str);
}

struct Sstr *Sstr_add(struct Sstr *str1, struct Sstr *str2) {
    struct Sstr *str = Sstr_new();

    str->string = Smem_Malloc(sizeof(char) * (str1->size + str2->size + 1));

    for (int i = 0; i < str1->size; i++) {
        str->string[i] = str1->string[i];
    }

    for (int i = 0; i < str2->size; i++) {
        str->string[i + str1->size] = str2->string[i];
    }

    str->size = str1->size + str2->size;

    return str;
}

struct Sstr *Sstr_mul(struct Sstr *str, int n) {
    struct Sstr *result = Sstr_new();

    result->string = Smem_Malloc(sizeof(char) * (str->size * n + 1));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < str->size; j++) {
            result->string[i * str->size + j] = str->string[j];
        }
    }

    result->size = str->size * n;

    return result;
}

struct Sobj* Sobj_make_string_object(struct Sstr *str) {
    struct Sobj *sobj = Sobj_new();
    sobj->type = STRING_OBJ;
    sobj->f_type = Stype_new();
    sobj->f_type->f_str = str;
    return sobj;
}

struct Sobj *Sobj_make_string(char* str, int size) {
    struct Sstr *sstr = Sstr_new_from_char(str, size);
    struct Sobj *sobj = Sobj_new();

    sobj->type = STRING_OBJ;
    sobj->f_type = Stype_new();
    sobj->f_type->f_str = sstr;

    return sobj;
}

struct Sobj *Sobj_make_char(char chr) {
    char* str = Sadd_1_char(chr);
    struct Sobj *sobj = Sobj_new();

    sobj->type = STRING_OBJ;
    sobj->f_type = Stype_new();
    sobj->f_type->f_str = Sstr_new_from_char(str, 1);

    return sobj;
}

int Sstr_cmp(struct Sstr *str1, struct Sstr *str2)
{
    if (!str1 || !str2) return 0;

    char *a = str1->string;
    char *b = str2->string;

    int size_a = str1->size;
    int size_b = str2->size;

    int min = size_a < size_b ? size_a : size_b;

    for (int i = 0; i < min; i++) {
        if (a[i] != b[i]) {
            return (unsigned char)a[i] - (unsigned char)b[i];
        }
    }

    return size_a - size_b;
}

struct Sstr *Sstr_mod(struct Sstr *str1, struct Sstr *str2) {
    struct Sstr *str = Sstr_new();

    str->string = Smem_Malloc(sizeof(char) * (str1->size + str2->size + 1));

    SmodChars(str->string, str1->string, str2->string);

    str->size = strlen(str->string);           

    return str;
}

struct Sstr *Sstr_cast_to_string(struct Sobj *obj) {
    struct Sstr *str = Sstr_new();
    char* new = Sio_sprintf(obj);
    str->string = new;
    str->size = strlen(new);
    return str;
}