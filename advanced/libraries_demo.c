#include <stdio.h>
#include <stdlib.h>
#include "libraries.h"

int main() {

    stack_t *stack = new_stack(5);
    printf("stack cap: %d, count: %d, address: %p\n",
        stack->capacity, stack->count, *stack->data);
    
    for (int i = 1; i < 10; i++) {
        int *heap_int = (int *)malloc(sizeof(int));
        *heap_int = i;
        int failure = push(stack, heap_int);
        if (failure) {
            printf("unable to push new data");
            free(heap_int);
            exit(1);
        }
        printf("stack cap: %d, count: %d, address: %p, idxv: %d - %d\n",
            stack->capacity,
            stack->count,
            *stack->data,
            i - 1,
            *(int *)stack->data[i - 1]);
    }
    for (int i = 1; i < 10; i++) {
        int *poped = (int *)pop(stack);
        if (!poped) {
            printf("stack is empty");
            break;
        }
        printf("poped: %d\n", *poped);
        free(poped);
    }
    free_stack(stack);
    return 0;
}