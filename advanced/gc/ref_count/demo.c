#include <stdio.h>
#include <string.h>
#include "object.h"

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