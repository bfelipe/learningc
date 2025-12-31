#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
/*
    Let's build a simple garbage collector
    on top of our tiny lang.
    The first thing we gonna do is to build a important
    component of any modern programming language.
    Objects.

    Objects are represetations of data on memory,
    this component is stored into a small container
    with metadata about itself.

    C does not have objects like modern languages.
    We going to build here some basic foundations
    for a few different data types that will be stored
    only on the heap, which later will be collected.


    A Garbage Collector (GC) is a program or subsystem that performs
    automatic memory management, specifically tasked with identifying 
    and freeing heap memory that is no longer accessible or "live" 
    by the running application.

    // --- BENEFITS ---
    Benefits include boosting the development process by eliminating the 
    need for manual memory deallocation (i.e., manual calls to free()), 
    thereby preventing common programming errors like memory leaks and 
    dangling pointers.

    // --- DOWNSIDES (The Trade-off) ---
    The primary drawback is performance overhead. Since the garbage collector 
    is a background process that must periodically execute to find and reclaim
    unused memory, it consumes CPU cycles and memory bandwidth. This often results 
    in temporary pauses (latency) in the main application's process speed 
    when the collector runs.
*/
typedef struct vm_t vm_t;
typedef struct object_t object_t;
typedef struct vector3_t vector3_t;
typedef struct array_t array_t;
typedef union data_t data_t;

typedef enum {
    INT,
    FLOAT,
    STRING,
    VECTOR3,
    ARRAY,
} type_t;

struct vector3_t{
    object_t *x;
    object_t *y;
    object_t *z;
};

struct array_t {
    int size;
    object_t **elements;
};

union data_t{
    int v_int;
    float v_float;
    char *v_string;
    vector3_t *v_vector3;
    array_t *v_array;
};

struct object_t {
    data_t data;
    int ref_count;
    type_t kind;
    bool is_marked;
};

object_t *new_obj(vm_t *vm);
object_t *new_int(vm_t *vm, int value);
object_t *new_float(vm_t *vm, float value);
object_t *new_string(vm_t *vm, char *value);
object_t *new_vector3(vm_t *vm, object_t *x, object_t *y, object_t *z);
object_t *new_array(vm_t *vm, int size);
void free_obj(object_t *obj);
bool arr_set(object_t *arr, object_t *obj, int idx);
object_t *arr_get(object_t *arr, int idx);
int len(object_t *obj);
void refcount_inc(object_t *obj);
void refcount_dec(object_t *obj);
/*
    If either input is NULL, return NULL.
    If "a" is an integer:
        If "b" is an integer, return a new_snek_integer with the sum of the two integers.
        If "b" is a float, return a new_snek_float with the sum of the integer and float.
        Anything else, invalid operation, return NULL.
    If "a" is a float:
        If "b" is an integer, return a new_snek_float with the sum of the float and integer.
        If "b" is a float, return a new_snek_float with the sum of the two floats.
        Anything else, invalid operation, return NULL.
    If "a" is a string:
        If "b" is not a string, invalid operation, return NULL.
        Otherwise:
        Calculate the length of the new string by combining the length of the two strings (properly handling the null terminator)
        Allocate memory for a new temporary string using calloc
        Use strcat to append the data from a and then b to the temporary string.
        Create a new_snek_string and pass in the temporary string.
        Free the memory for the temporary string and return the new string object.
    If "a" is a vector3:
        If "b" is not a VECTOR3, invalid operation, return NULL.
        Otherwise:
        Create a new_snek_vector3.
        Recursively call snek_add for each of the x, y, and z fields. For example, a vector [1,2,3]+[4,5,6] should result in a new vector [5,7,9].
        Return the new vector struct.
    If "a" is an array:
        If "b" is not an array, invalid operation, return NULL.
        Otherwise:
        Create a new_snek_array with the combined length of the two arrays.
        Iterate over each index in "a" and use snek_array_set and snek_array_get to copy the values from "a" to the new array.
        Do the same for "b".
        Return the new array object.
    If "a" is none of the above, invalid operation, return NULL.
int add(object_t *a, object_t *b);
int sub(object_t *a, object_t *b);
int mul(object_t *a, object_t *b);
int div(object_t *a, object_t *b);
int mod(object_t *a, object_t *b);
*/