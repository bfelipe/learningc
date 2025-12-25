#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "object.h"

object_t *new_obj(void) {
    object_t *obj = (object_t *)calloc(1, sizeof(object_t));
    if (!obj) {
        return NULL;
    }
    obj->ref_count = 1;
    return obj;
}

object_t *new_int(int value) {
    object_t *obj = new_obj();
    if (!obj) {
        return NULL;
    }
    obj->data.v_int = value;
    obj->kind = INT;
    return obj;
}

object_t *new_float(float value) {
    object_t *obj = new_obj();
    if (!obj) {
        return NULL;
    }
    obj->data.v_float = value;
    obj->kind = FLOAT;
    return obj;
}

object_t *new_string(char *value) {
    object_t *obj = new_obj();
    if (!obj) {
        return NULL;
    }
    char *data = (char *)malloc(strlen(value) + 1);
    if (!data) {
        free(obj);
        return NULL;
    }
    strcpy(data, value);
    obj->data.v_string = data;
    obj->kind = STRING;
    return obj;
}

object_t *new_vector3(object_t *x, object_t *y, object_t *z) {
    object_t *obj = new_obj();
    if (!obj) {
        return NULL;
    }
    vector3_t *vec = (vector3_t *)malloc(sizeof(vector3_t));
    if (!vec) {
        return NULL;
    }
    vec->x = x;
    vec->y = y;
    vec->z = z;
    refcount_inc(x);
    refcount_inc(y);
    refcount_inc(z);
    obj->data.v_vector3 = vec;
    obj->kind = VECTOR3;
    return obj;
}

object_t *new_array(int size) {

    object_t *obj = new_obj();
   if (!obj) {
    return NULL;
   }
   array_t *arr = (array_t *)malloc(sizeof(array_t));
   if (!arr) {
    free(obj);
    return NULL;
   }
   object_t **elements = (object_t **)calloc(size, sizeof(object_t *));
   if (!elements) {
    free(arr);
    free(obj);
    return NULL;
   }
   arr->size = size;
   arr->elements = elements;
   obj->data.v_array = arr;
   obj->kind = ARRAY;
   return obj;
}

bool arr_set(object_t *arr, object_t *obj, int idx) {
    if (!arr || !obj) return false;
    if (idx < 0 || idx > (arr->data.v_array->size - 1)) return false;
    if (arr->kind != ARRAY) return false;
    if (arr->data.v_array->elements[idx]) {
        refcount_dec(arr->data.v_array->elements[idx]);
    }
    refcount_inc(obj);
    arr->data.v_array->elements[idx] = obj;
    return true;
}

object_t *arr_get(object_t *arr, int idx) {
    if (!arr) return NULL;
    if (arr->kind != ARRAY) return NULL;
    if (!arr->data.v_array) return NULL;
    if (idx < 0 || idx > (arr->data.v_array->size - 1)) return NULL;
    return arr->data.v_array->elements[idx];
}

int len(object_t *obj) {
    if (!obj) return 0;
    switch (obj->kind) {
        case VECTOR3:
            return 3;
        case ARRAY:
            return obj->data.v_array->size;
        case STRING:
            return strlen(obj->data.v_string) + 1;
        default:
            return -1;
    }
}

void free_obj(object_t *obj) {
    if (!obj) return;
    switch (obj->kind) {
    case INT:
    case FLOAT:
        break;
    case STRING:
        if (obj->data.v_string) {
            free(obj->data.v_string);
            obj->data.v_string = NULL;
        }
        break;
    case VECTOR3:
        if (obj->data.v_vector3) {
            refcount_dec(obj->data.v_vector3->x);
            refcount_dec(obj->data.v_vector3->y);
            refcount_dec(obj->data.v_vector3->z);
            free(obj->data.v_vector3);
            obj->data.v_vector3 = NULL;
        }
        break;
    case ARRAY:
        if (obj->data.v_array) {
            if (obj->data.v_array->elements) {
                for (int i = 0; i < obj->data.v_array->size; i++) {
                    refcount_dec(obj->data.v_array->elements[i]);
                    obj->data.v_array->elements[i] = NULL;
                }
                free(obj->data.v_array->elements);
            }
            free(obj->data.v_array);
        }
        break;
    default:
        printf("fail to identify object type");
        return;
    }
    free(obj);
}

void refcount_inc(object_t *obj) {
    if (!obj) return;
    obj->ref_count += 1;
}

void refcount_dec(object_t *obj) {
    if (!obj) return;
    obj->ref_count -= 1;
    if (obj->ref_count > 0) return;
    free_obj(obj);
}