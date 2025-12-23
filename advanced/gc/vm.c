#include <stdlib.h>
#include "vm.h"
#include "object.h"

stack_t *new_stack(size_t cap) {
    stack_t *stack = (stack_t *)calloc(1, sizeof(stack_t));
    if (!stack) return NULL;
    object_t **data = (object_t **)calloc(cap, sizeof(object_t *));
    if (!data) {
        free(stack);
        return NULL;
    }
    stack->data = data;
    stack->cap = cap;
    return stack;
}

void free_stack(stack_t *ptr) {
    if (!ptr) return;
    if (ptr->data) {
        for (int i = 0; i < ptr->count; i ++) {
            refcount_dec(ptr->data[i]);
        }
        free(ptr->data);
    }
    free(ptr);
}

int push(stack_t *ptr, object_t *obj) {
    if (!ptr || !obj) return 1;
    if (ptr->cap == ptr->count) {
        size_t new_cap = ptr->cap * 2;
        size_t new_size_bytes = new_cap * sizeof(object_t *);
        object_t **new_data = realloc(ptr->data, new_size_bytes);
        if (!new_data) {
            printf("fail to reallocate stack on the heap");
            return 1;
        }
        memset(
            new_data + ptr->count,
            0,
            (new_cap - ptr->cap) * sizeof(object_t *));
        ptr->data = new_data;
        ptr->cap = new_cap;
    }
    refcount_inc(obj);
    ptr->data[ptr->count] = obj;
    ptr->count += 1;
    return 0;
}

object_t *pop(stack_t *ptr) {
    if (!ptr || ptr->count == 0) return NULL;
    ptr->count -= 1;
    object_t *data = ptr->data[ptr->count];
    return data;
}

vm_t *new_vm() {
    vm_t *vm = (vm_t *)calloc(1, sizeof(vm_t));
    if (!vm) {
        return NULL;
    }
    vm->frames = new_stack(8);
    vm->objects = new_stack(8);
    if (!vm->frames || !vm->objects) {
        free_vm(vm);
        return NULL;
    }
    return vm;
}

void free_vm(vm_t *vm) {
    if (!vm) return;
    if (vm->frames) {
        free_stack(vm->frames);
    }
    if (vm->objects) {
        free_stack(vm->objects);
    }
    free(vm);
}