#include "slist.h"
#include "smem.h"
#include "sdebug.h"

int Slist_free(struct Slist* list) {
    for (int i = 0; i < list->count; i++) {
        struct Sobj* obj = list->array[i];
        _SUNYDECREF(obj);

        if (obj->ref < 1) {
            Sobj_free(obj);
        }
    }
    
    Smem_Free(list->array);
    Smem_Free(list);

    return 0;
}

struct Slist* Slist_new(void) {
    struct Slist* list = Smem_Malloc(sizeof(struct Slist));
    list->capacity = 1024;
    list->count = 0;
    list->array = Smem_Calloc(list->capacity, sizeof(struct Sobj*));
    return list;
}

struct Slist* Slist_add(struct Slist* list, struct Sobj* obj) {
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->array = Smem_Realloc(list->array, sizeof(struct Sobj*) * list->capacity);
    }

    list->array[list->count++] = obj;
    return list;
}

struct Sobj* Slist_get(struct Slist* list, int index) {
    if (list->array[index]) {
        return list->array[index];
    } else {
        return NULL;
    }
}

struct Sobj* Sobj_make_list(struct Slist* list) {
    struct Sobj* obj = Sobj_new();
    obj->type = LIST_OBJ;
    obj->f_type = Stype_new();
    obj->f_type->f_list = list;
    return obj;
}

struct Slist* Slist_change_item(struct Slist* list, int index, struct Sobj* obj) {
    list->array[index] = obj;
    return list;
}

struct Slist* Slist_pop(struct Slist* list) {
    list->array[list->count - 1] = NULL;
    list->count -= 1;
    return list;
}

struct Slist* Slist_append(struct Slist* list1, struct Slist* list2) {
    struct Slist* list = Slist_new();

    for (int i = 0; i < list1->count; i++) {
        Slist_add(list, list1->array[i]);
        _SUNYINCREF(list1->array[i]);
    }

    for (int i = 0; i < list2->count; i++) {
        Slist_add(list, list2->array[i]);
        _SUNYINCREF(list2->array[i]);
    }

    return list;
}

struct Slist* Slist_mul(struct Slist* list1, int num) {
    struct Slist* list = Slist_new();

    for (int i = 0; i < num; i++) {
        for (int j = 0; j < list1->count; j++) {
            Slist_add(list, list1->array[j]);
            _SUNYINCREF(list1->array[j]);
        }
    }

    return list;
}

struct Slist* Slist_range(int start, int end) {
    struct Slist* list = Slist_new();

    if (start > end) {
        for (int i = end; i < start; i++) {
            struct Sobj* value = Sobj_make_number(i);
            _SUNYINCREF(value);
            Slist_add(list, value);
        }
    } else {
        for (int i = start; i < end; i++) {
            struct Sobj* value = Sobj_make_number(i);
            _SUNYINCREF(value);
            Slist_add(list, value);
        }
    }

    return list;
}

struct Slist *Slist_from_string_chars(char *str) {
    struct Slist *list = Slist_new();
    
    for (int i = 0; str[i] != '\0'; i++) {
        char current_char = str[i];
        
        char char_str[2];
        char_str[0] = current_char;
        char_str[1] = '\0';
        
        struct Sobj *value = Sobj_make_string(char_str, 1);
        
        _SUNYINCREF(value);

        Slist_add(list, value);
    }
    
    return list;
}

struct Slist *Slist_store(struct Slist *list, int index, struct Sobj *obj) {
    SDEBUG("[slist.c] Slist_store(struct Slist *list, int index, struct Sobj *obj) (storing...)\n");

    if (index >= list->capacity) {
        list->capacity *= 2;
        list->array = (struct Sobj**)Smem_Realloc(list->array, sizeof(struct Sobj*) * list->capacity);
    }

    if (index >= list->count) {
        for (int i = list->count; i <= index; ++i) {
            Slist_add(list, null_obj);
        }
    }

    list->array[index] = obj;

    SDEBUG("[slist.c] Slist_store(struct Slist *list, int index, struct Sobj *obj) (done)\n");
    return list;
}

int Slist_cmp(struct Slist* list1, struct Slist* list2) {
    int min = list1->count < list2->count ? list1->count : list2->count;

    for (int i = 0; i < min; i++) {
        struct Sobj* a = list1->array[i];
        struct Sobj* b = list2->array[i];

        if (!a || !b) __ERROR("null element in list compare");

        if (Scompare(a, b, CMP_LT)) return -1;

        if (Scompare(a, b, CMP_GT)) return 1;
    }

    if (list1->count < list2->count) return -1;
    if (list1->count > list2->count) return 1;

    return 0;
}
