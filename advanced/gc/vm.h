#pragma once
#include "object.h"
/*
    We are going to expand our refcount garbabe collector
    using another algorithm called mark and swep.
    Here are some pos and cons of using such algorithm

    Pos
    - can detect cycle
    - less book keeping
    - reduce potential performance degradation

    Cons
    - complex
    - stop-the-world when lots of objects exists and must be free(affect performance)
    - high memory overhead
    - less predicable performance
*/

typedef struct {
    int count;
    int cap;
    object_t **data;
} stack_t;

stack_t *new_stack(size_t cap);
void free_stack(stack_t *ptr);
int push(stack_t *ptr, object_t *obj);
object_t *pop(stack_t *ptr);

typedef struct {
    /*
        VM stands for virtual machine.
        We are going to implement a very simple one,
        as we are going to use it to track objets that will live in the stack frame
        and on the heap
    */
    stack_t *frames; // frame scope
    /*
        Here is an example of a scope frame in python code

        mgs = 'scope 1'
        def my_func():
            msg = 'scope 2'
            def inner_func():
                mgs = 'scope 3'
                return
            return
    */
    stack_t *objects;
} vm_t;

vm_t *new_();
void free_vm(vm_t *vm);