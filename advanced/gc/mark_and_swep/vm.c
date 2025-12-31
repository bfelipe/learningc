#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vm.h"
#include "object.h"

stack_t *new_stack(size_t cap) {
    stack_t *stack = (stack_t *)calloc(1, sizeof(stack_t));
    if (!stack) return NULL;
    void **data = (void **)calloc(cap, sizeof(void *));
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
        free(ptr->data);
    }
    free(ptr);
}

int push(stack_t *ptr, void *obj) {
    if (!ptr || !obj) return 1;
    if (ptr->cap == ptr->count) {
        size_t new_cap = ptr->cap * 2;
        size_t new_size_bytes = new_cap * sizeof(void *);
        void **new_data = realloc(ptr->data, new_size_bytes);
        if (!new_data) {
            printf("fail to reallocate stack on the heap");
            return 1;
        }
        memset(
            new_data + ptr->count,
            0,
            (new_cap - ptr->cap) * sizeof(void *));
        ptr->data = new_data;
        ptr->cap = new_cap;
    }
    ptr->data[ptr->count] = obj;
    ptr->count += 1;
    return 0;
}

void *pop(stack_t *ptr) {
    if (!ptr || ptr->count == 0) return NULL;
    ptr->count -= 1;
    void *data = ptr->data[ptr->count];
    return data;
}

stack_t *remove_null(stack_t *stack) {
    stack_t *cln_stack = new_stack(stack->cap);
    for (int i = 0; i < stack->count; i++) {
        object_t *obj = stack->data[i];
        if (obj == NULL) continue;
        push(cln_stack, obj);
    }
    return cln_stack;
}

frame_t *new_frame(vm_t *vm) {
    if (!vm) return NULL;
    frame_t *frame = (frame_t *)calloc(1, sizeof(frame_t));
    if (!frame) return NULL;
    frame->reference = new_stack(VM_INIT_STACK_SIZE);
    if (!frame->reference) {
        free(frame);
        return NULL;
    }
    return frame;
}

void push_frame(vm_t *vm, frame_t *frame) {
    if (!vm || !vm->frames || !frame) return;
    push(vm->frames, frame);
}

void free_frame(frame_t *frame) {
    if (!frame) return;
    free_stack(frame->reference);
    free(frame);
}

void track_obj(vm_t *vm, void *obj) {
    if (!vm || !vm->objects) return;
    push(vm->objects, obj);
}

vm_t *new_vm(void) {
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
        frame_t *f;
        while ((f = pop(vm->frames)) != NULL) {
            free_frame(f);
        }
        free_stack(vm->frames);
    }
    if (vm->objects) {
        object_t *obj;
        while ((obj = pop(vm->objects)) != NULL) {
            destroy(obj);
        }
        free_stack(vm->objects);
    }
    free(vm);
}

void destroy(object_t *obj) {
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
            free(obj->data.v_vector3);
            obj->data.v_vector3 = NULL;
        }
        break;
    case ARRAY:
        if (obj->data.v_array) {
            if (obj->data.v_array->elements) {
                free(obj->data.v_array->elements);
                obj->data.v_array->elements = NULL;
            }
            free(obj->data.v_array);
            obj->data.v_array = NULL;
        }
        break;
    default:
        printf("fail to identify object type");
        return;
    }
    free(obj);
}

void frame_ref(frame_t *frame, object_t *obj) {
    if (!frame || !obj) return;
    push(frame->reference, obj);
}

void mark(vm_t *vm) {
    if (!vm || !vm->frames) return;
    for (int i = 0; i < vm->frames->count; i++) {
        frame_t *frame = vm->frames->data[i];
        for (int j = 0; j < frame->reference->count; j++) {
            object_t *obj = frame->reference->data[j];
            obj->is_marked = true;
        }
    }
}

/*
    Trace solves a set of problem related to inner reference or
    cyclic reference, where some objects which suppose to still be used
    are free, and once that objects is tried to be accessed some unexpected
    behavior happens, since that object no longer exists, but still considered
    alive.
    In other words, tracing mark objects references by or root objects
*/
void trace(vm_t *vm) {
    if(!vm || !vm->objects) return;
    stack_t *gray_obj = new_stack(VM_INIT_STACK_SIZE);
    for (int i = 0; i < vm->objects->count; i++) {
        object_t *obj = vm->objects->data[i];
        if (obj->is_marked) {
            push(gray_obj, obj);
        }
    }
    while (gray_obj->count > 0) {
        object_t * obj = pop(gray_obj);
        trace_blacken_obj(gray_obj, obj);
    }
    free_stack(gray_obj);
}

void trace_blacken_obj(stack_t *gray_obj, object_t *obj) {
    if (!obj) return;
    switch (obj->kind) {
        case INT:
        case FLOAT:
        case STRING:
            break;
        case VECTOR3:
            trace_mark_obj(gray_obj, obj->data.v_vector3->x);
            trace_mark_obj(gray_obj, obj->data.v_vector3->y);
            trace_mark_obj(gray_obj, obj->data.v_vector3->z);
            break;
        case ARRAY:
            for (int i = 0; i < obj->data.v_array->size; i++) {
                trace_mark_obj(gray_obj, obj->data.v_array->elements[i]);
            }
            break;
        default: break;
    }
}

void trace_mark_obj(stack_t *gray_obj, object_t *obj) {
    if (!obj || obj->is_marked == true) return;
    obj->is_marked = true;
    push(gray_obj, obj);
}

void swep(vm_t *vm) {
    if (!vm || !vm->objects) return;
    for (int i = 0; i < vm->objects->count; i++) {
        object_t *obj = vm->objects->data[i];
        if (obj->is_marked) {
            obj->is_marked = false;
        } else {
            destroy(obj);
            vm->objects->data[i] = NULL;
        }
    }
    stack_t *old = vm->objects;
    vm->objects = remove_null(old);
    free(old->data);
    free(old);
}

void collect_garbage(vm_t *vm) {
    mark(vm);
    trace(vm);
    swep(vm);
}