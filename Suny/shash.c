#include "shash.h"

static uint64_t mix64(uint64_t x) {
    x ^= x >> 30;
    x *= 0xbf58476d1ce4e5b9ULL;
    x ^= x >> 27;
    x *= 0x94d049bb133111ebULL;
    x ^= x >> 31;
    return x;
}

hash_t Shash(struct Sobj* obj) {
    switch (TypeOf(obj)) {
        case NUMBER_OBJ:
            return Shash_number(obj);
        case STRING_OBJ:
            return Shash_string(obj);
        case TRUE_OBJ:
            return Shash_true(obj);
        case FALSE_OBJ:
            return Shash_false(obj);
        case CLASS_OBJ:  
            return Shash_class(obj);
        case FUNC_OBJ:  
            return Shash_function(obj);
        case NULL_OBJ:
            return NULL_HASH;
    }

    return ERROR_HASH;
}

hash_t Shash_number(struct Sobj* obj) {
    if (TypeOf(obj) != NUMBER_OBJ) return ERROR_HASH;

    Number_t number = ValueOf(obj);

    if (number == 0.0) return 0;

    uint64_t bits;
    memcpy(&bits, &number, sizeof(Number_t));

    bits ^= bits >> 33;
    bits *= 0xff51afd7ed558ccdULL;
    bits ^= bits >> 33;
    bits *= 0xc4ceb9fe1a85ec53ULL;
    bits ^= bits >> 33;

    return bits;
}

hash_t Shash_string(struct Sobj* obj) {
    if (TypeOf(obj) != STRING_OBJ) return ERROR_HASH;

    struct Sstr* s = tget_str(obj);
    const unsigned char* str = (const unsigned char*)s->string;

    hash_t hash = 1469598103934665603ULL;

    for (int i = 0; i < s->size; i++) {
        hash ^= str[i];
        hash *= 1099511628211ULL;
    }

    return hash;
}

hash_t Shash_true(struct Sobj* obj) {
    if (TypeOf(obj) != TRUE_OBJ)
        return ERROR_HASH;

    return TRUE_HASH;
}

hash_t Shash_false(struct Sobj* obj) {
    if (TypeOf(obj) != FALSE_OBJ)
        return ERROR_HASH;

    return FALSE_HASH;
}

hash_t Shash_class(struct Sobj* obj) {
    if (TypeOf(obj) != CLASS_OBJ) return ERROR_HASH;

    uint64_t addr = (uint64_t)(uintptr_t)obj;
    return mix64(addr);
}

hash_t Shash_function(struct Sobj* obj) {
    if (TypeOf(obj) != FUNC_OBJ) return ERROR_HASH;

    uint64_t addr = (uint64_t)(uintptr_t)obj;
    return mix64(addr);
}