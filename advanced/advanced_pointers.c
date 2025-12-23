#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Now we know pointer are just a way to store the address
    of a given variable.
    But we can also store the address of a pointer, through
    pointer of pointers
*/
int allocate_int(int **pointer_pointer, int value) {
    int *pointer = (int *)calloc(1, sizeof(int));
    if (!pointer) {
        printf("fail to allocate int");
        exit(1);
    }
    *pointer_pointer = pointer; // store address from heap
    **pointer_pointer = value; // de-reference value
}

/*
    We can also create an array of pointes using malloc or 
    calloc.
*/
int ** allocate_int_ptr_arr(int *int_arr, size_t size) {
    int **int_ptr_arr = (int **)calloc(size, sizeof(int **));
    if (!int_ptr_arr) {
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        int *ptr = (int *)calloc(1, sizeof(int *));
        if (!ptr) {
            exit(1);
        }
        int_ptr_arr[i] = ptr;
        *int_ptr_arr[i] = int_arr[i];
    }
    return int_ptr_arr;
}


/*
    Void pointers are generic pointers that must be
    cast before used for a specific type.
*/
typedef enum Types {
    INT,
    FLOAT,
    STRING,
} types_t;

void printValue(void *ptr, types_t type) {
    switch (type) {
        case INT:
            printf("%d\n", *(int *)ptr);
            break;
        case FLOAT:
            printf("%f\n", *(float *)ptr);
            break;
        case STRING:
            printf("%d\n", *(char *)ptr);
            break;
        default:
            printf("can't de-reference pointer value\n");
    }
}

/*
    We can also use pointer to swap values of a given memory addresses
*/
void swap_int(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void swap_str(char **a, char **b) {
    char *tmp = *a;
    *a = *b;
    *b = tmp;
}

/*
    We can also perform genery swap by giving to the compiler
    the size of the data we want to swap.
    We also going to need to use memcpy to swap the content
    of both pointers
*/
void swap(void *a, void *b, size_t size ) {
    void *tmp = malloc(size);
    if (!tmp) {
        printf("fail to allocate memory");
        exit(1);
    }
    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);
    free(tmp);
}

int main() {

    int value = 10;
    int *pointer = &value;
    printf("value %d, pointer %p\n", value, pointer);
    allocate_int(&pointer, 20);
    printf("value %d, pointer %p\n", *pointer, pointer);
    free(pointer);

    int size = 3;
    int int_arr[3] = {1, 2 , 3};
    int **int_arr_ptr = allocate_int_ptr_arr(int_arr, size);
    for (int i = 0; i < size; i++) {
        // print og value and addr, in comparison to heap allocated value and addr
        printf("value %d, ptr %p, value %d, arr_ptr %p\n", int_arr[i], &int_arr[i], *int_arr_ptr[i], int_arr_ptr[i]);
        free(int_arr_ptr[i]);
    }
    
    int *intptr = &value;
    printValue((void *)intptr, INT);

    int a = 10, b = 20;
    printf("a %d, b %d\n", a, b);
    swap_int(&a, &b);
    printf("a %d, b %d\n", a, b);

    char *c = "allan", *d = "turing";
    printf("c %s, d %s\n", c, d);
    swap_str(&c, &d);
    printf("c %s, d %s\n", c, d);

    printf("--- Generic Swap ---\n");
    swap(&c, &d, sizeof(char *));
    printf("c %s, d %s\n", c, d);
    swap(&a, &b, sizeof(int *));
    printf("a %d, b %d\n", a, b);
    return 0;
}