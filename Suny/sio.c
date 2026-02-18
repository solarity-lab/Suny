#include "sio.h"
#include "smem.h"
#include "sobj.h"
#include "stype.h"

int Sio_write(struct Sobj *obj) {
    if (!obj)
        printf("NULL OBJECT");

    if (TypeOf(obj) == STRING_OBJ) {
        printf("%s", obj->f_type->f_str->string);
    } else if (TypeOf(obj) == LIST_OBJ) {
        printf("[");
        for (int i = 0; i < obj->f_type->f_list->count; i++) {
            struct Sobj *item = obj->f_type->f_list->array[i];
            if (item->type == STRING_OBJ) {
                printf("\"");
                Sio_write(item);
                printf("\"");
            } else {
                Sio_write(item);
            }
            if (i < obj->f_type->f_list->count - 1) {
                printf(", ");
            }
        }
        printf("]");
    } else if (TypeOf(obj) == TRUE_OBJ) {
        printf("true");
    } else if (TypeOf(obj) == FALSE_OBJ) {
        printf("false");
    } else if (TypeOf(obj) == FUNC_OBJ) {
        printf("<function object %p defined in Suny VM at address %i>", obj->f_type->f_func, obj->address);
    } else if (TypeOf(obj) == USER_DATA_OBJ) {
        if (obj->meta && obj->meta->mm_tostring) {
            obj->meta->mm_tostring(obj);
        } else {
            printf("<user data object %p defined in Suny VM at address %i>", obj->f_type->f_userdata, obj->address);
        }
    } else if (TypeOf(obj) == CLASS_OBJ) {
        printf("<class object %p defined in Suny VM at address %i>", obj->f_type->f_class, obj->address);
    } else if (TypeOf(obj) == MAP_OBJ) {
        struct Shashmap* map = tget_hashmap(obj);

        printf("{");
        int f = 1;

        for (int i = 0; i < map->capacity; i++) {
            struct Shash_entry* entry = map->buckets[i];

            while (entry) {
                if (!f)
                    printf(", ");

                Sio_write(entry->key);
                printf(": ");
                Sio_write(entry->value);

                f = 0;
                entry = entry->next;
            }
        }

        printf("}");
    } else if (TypeOf(obj) == NULL_OBJ) {
        printf("null");
    } else {
        printf("%g", obj->value->value);
    }

    return 0;
}

char* Sio_sprintf(struct Sobj* obj) {
    char* str = Smem_Malloc(1024);

    if (TypeOf(obj) == STRING_OBJ) {
        strcpy(str, obj->f_type->f_str->string);
    } else if (TypeOf(obj) == LIST_OBJ) {
        strcpy(str, "[");
        for (int i = 0; i < obj->f_type->f_list->count; i++) {
            struct Sobj *item = obj->f_type->f_list->array[i];
            if (item->type == STRING_OBJ) {
                strcat(str, "\"");
                strcat(str, item->f_type->f_str->string);
                strcat(str, "\"");
            } else {
                strcat(str, Sio_sprintf(item));
            }

            if (i < obj->f_type->f_list->count - 1) {
                strcat(str, ", ");
            }
        }
        strcat(str, "]");
    } else if (TypeOf(obj) == TRUE_OBJ) {
        strcpy(str, "true");
    } else if (TypeOf(obj) == FALSE_OBJ) {
        strcpy(str, "false");
    } else if (TypeOf(obj) == NULL_OBJ) {
        strcpy(str, "null");
    } else {
        char* buffer = Sstring("%g", obj->value->value);
        strcpy(str, buffer);
        Smem_Free(buffer);
    }

    return str;
}
