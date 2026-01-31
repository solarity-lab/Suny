#include "sio.h"
#include "smem.h"

int Sio_write(struct Sobj *obj) {
    if (obj->type == STRING_OBJ) {
        printf("%s", obj->f_type->f_str->string);
    } else if (obj->type == LIST_OBJ) {
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
    } else if (obj->type == TRUE_OBJ) {
        printf("true");
    } else if (obj->type == FALSE_OBJ) {
        printf("false");
    } else if (obj->type == FUNC_OBJ) {
        printf("<function object %p defined in Suny VM at address %i>", obj->f_type->f_func, obj->address);
    } else if (obj->type == USER_DATA_OBJ) {
        if (obj->meta && obj->meta->mm_tostring) {
            obj->meta->mm_tostring(obj);
        } else {
            printf("<user data object %p defined in Suny VM at address %i>", obj->f_type->f_userdata, obj->address);
        }
    } else if (obj->type == CLASS_OBJ) {
        printf("<class object %p defined in Suny VM at address %i>", obj->f_type->f_class, obj->address);
    } else if (obj->type == NULL_OBJ) {
        printf("null");
    } else {
        printf("%g", obj->value->value);
    }

    return 0;
}

char* Sio_sprintf(struct Sobj* obj) {
    char* str = Smem_Malloc(1024);

    if (obj->type == STRING_OBJ) {
        strcpy(str, obj->f_type->f_str->string);
    } else if (obj->type == LIST_OBJ) {
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
    } else if (obj->type == TRUE_OBJ) {
        strcpy(str, "true");
    } else if (obj->type == FALSE_OBJ) {
        strcpy(str, "false");
    } else if (obj->type == NULL_OBJ) {
        strcpy(str, "null");
    } else {
        char* buffer = Sstring("%g", obj->value->value);
        strcpy(str, buffer);
        Smem_Free(buffer);
    }

    return str;
}