#pragma once
#include <stdlib.h>
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
#define VM_INIT_STACK_SIZE 8
typedef struct object_t object_t;
typedef struct stack_t stack_t;
typedef struct frame_t frame_t;
typedef struct vm_t vm_t;

struct stack_t{
    int count;
    int cap;
    void **data;
};

struct frame_t {
    stack_t *reference;
};

/*
    Mark-and-sweep is an garbage-collection algorithm 1960s.
    It has two phases: 
    
    mark — traverse the object graph starting from 
    the root set and mark all reachable objects;
    
    sweep — reclaim all unmarked objects. Unlike reference
    counting, it does not track per-object counts; instead
    it finds live objects by tracing from roots (stack, globals, registers,
    VM root lists), which also allows it to collect cyclic garbage.
*/

struct vm_t{
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
};

vm_t *new_vm(void);
void free_vm(vm_t *vm);

stack_t *new_stack(size_t cap);
void free_stack(stack_t *ptr);
int push(stack_t *ptr, void *obj);
void *pop(stack_t *ptr);
stack_t *remove_null(stack_t *stack);

frame_t *new_frame(vm_t *vm);
void push_frame(vm_t *vm, frame_t *frame);
void free_frame(frame_t *frame);

void destroy(object_t *obj);
void track_obj(vm_t *vm, void *obj);
void frame_ref(frame_t *frame, object_t *obj);

void mark(vm_t *vm);
void trace(vm_t *vm);
void swep(vm_t *vm);

void trace_blacken_obj(stack_t *gray_obj, object_t *obj);
void trace_mark_obj(stack_t *gray_obj, object_t *obj);
void collect_garbage(vm_t *vm);