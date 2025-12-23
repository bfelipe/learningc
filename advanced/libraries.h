#pragma once

#include <stdlib.h>

typedef struct {
    int count;
    int capacity;
    void **data;
} stack_t;

stack_t *new_stack(size_t cap);
void free_stack(stack_t *ptr);
int push(stack_t *stack, void *obj);
void *pop(stack_t *stack);
