#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libraries.h"

/*
    In this module we going to implement a simple library, using generics in
    C. while managing generic pointers using void *.
    We going to implement a simple but robust stack collection, that can
    use any type.
*/
stack_t *new_stack(size_t cap) {
    stack_t *stack = malloc(sizeof(stack_t));
    if (!stack) {
        printf("fail to allocate memory");
        return NULL;
    }
    stack->count = 0;
    stack->capacity = cap;
    stack->data = (void **)malloc(cap * sizeof(void *));
    if (!stack->data) {
        printf("fail to allocate memory for stack data");
        free(stack);
        return NULL;
    }
    return stack;
}

void free_stack(stack_t *ptr) {
    if (!ptr) return;
    if (ptr->data) {
        for (int i = 0; i < ptr->count; i++) {
            if (ptr->data[i]) {
                free(ptr->data[i]);
                ptr->data[i] = NULL;
            }
        }
        free(ptr->data);
    }
    free(ptr);
}

/*
    Takes a stack pointer, and a object pointer.
    Returns 0 - success
    Returns 1 - failure    
*/
int push(stack_t *stack, void *obj) {
    if (stack->count == stack->capacity) {
        size_t new_cap = stack->capacity * 2;
        size_t new_size_bytes = new_cap * sizeof(void *);
        void *new_data = realloc(stack->data, new_size_bytes);
        if (!new_data) {
            printf("fail to reallocate stack on the heap");
            return 1;
        }
        stack->data = new_data;
        stack->capacity = new_cap;
    }
    stack->data[stack->count] = obj;
    stack->count += 1;
    return 0;
}

void *pop(stack_t *stack) {
    if (stack->count == 0) {
        return NULL;
    }
    stack->count -= 1;
    void *data = stack->data[stack->count];
    stack->data[stack->count] = NULL;
    return data;
}