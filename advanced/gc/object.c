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

int main() {
    object_t *obj_i = new_int(20);
    if (!obj_i) {
        printf("fail to allocate new int of object_t");
        return 1;
    }
    printf("object_t kind: %d, data: %d, %zu bytes\n",
        obj_i->kind, obj_i->data.v_int, sizeof(obj_i));
    printf("len: %d\n", len(obj_i));
    free_obj(obj_i);

    object_t *obj_f = new_float(20.2);
    if (!obj_f) {
        printf("fail to allocate new float of object_t");
        return 1;
    }
    printf("object_t kind: %d, data: %.1f, %zu bytes\n",
        obj_f->kind, obj_f->data.v_float, sizeof(obj_f));
    printf("len: %d\n", len(obj_f));
    free_obj(obj_f);

    object_t *obj_s = new_string("hello world");
    if (!obj_s) {
        printf("fail to allocate new float of object_t");
        return 1;
    }
    printf("object_t kind: %d, data: %s, obj size: %zu bytes, text size: %zu bytes\n", 
        obj_s->kind, obj_s->data.v_string, sizeof(obj_s), strlen(obj_s->data.v_string));
    printf("len: %d\n", len(obj_s));
    free_obj(obj_s);

    object_t *x = new_int(10);
    object_t *y = new_int(20);
    object_t *z = new_int(30);
    if (!x || !y || !z) {
        if (x) free_obj(x);
        if (y) free_obj(y);
        if (z) free_obj(z);
        return 1;
    }
    object_t *vec3 = new_vector3(x, y, z);
    if (!vec3) {
        free_obj(x);
        free_obj(y);
        free_obj(z);
        return 1;
    }
    refcount_dec(x);
    refcount_dec(y);
    refcount_dec(z);

    printf("object_t kind: %d, data: %d, %d, %d, %zu bytes\n",
        vec3->kind,
        vec3->data.v_vector3->x->data.v_int,
        vec3->data.v_vector3->y->data.v_int,
        vec3->data.v_vector3->z->data.v_int,
        sizeof(vec3));
    printf("len: %d\n", len(vec3));
    free_obj(vec3);

    int arr_size = 5;
    object_t *obj_arr = new_array(arr_size);
    if (!obj_arr) {
        return 1;
    }
    printf("object_t kind: %d, arr size: %d, data_addr: %p, obj size: %zu bytes\n",
        obj_arr->kind,
        obj_arr->data.v_array->size,
        obj_arr->data.v_array->elements,
        sizeof(obj_arr));
    for (int i = 0; i < 5; i++) {
        object_t *tmp = new_int(i * 2);
        if (!tmp) {
            free_obj(obj_arr);
            return 1;
        }
        bool success = arr_set(obj_arr, tmp, i);
        if (success == false) {
            printf("fail to insert %d\n", i * 2);
            free_obj(tmp);
            free_obj(obj_arr);
            return 1;
        }
        refcount_dec(tmp);
        printf("inserted %d\n", i * 2);
    }
    for (int i = 0; i < arr_size; i++) {
        printf("idx: %d - %d\n", i, arr_get(obj_arr, i)->data.v_int);
    }
    printf("len: %d\n", len(obj_arr));
    free_obj(obj_arr);

    object_t *generic_obj = new_obj();
    if (!generic_obj) {
        printf("fail to allocate new generic of object_t");
        return 1;
    }
    printf("object_t kind: %d, data: %d, %zu bytes ref_count: %d\n",
        generic_obj->kind, generic_obj->data.v_int, sizeof(generic_obj), generic_obj->ref_count);
    printf("len: %d\n", len(generic_obj));
    free_obj(generic_obj);   

    return 0;
}