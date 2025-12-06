#include <stdio.h>
#include <stdlib.h>

/*
    Unions are a special type of structure where all members
    occupy the same memory location.
    
    The compiler determines the size of the union object by
    finding the memory space required for the **largest** member type.
    
    Keep in mind, a union can store a value for **one** member 
    of the provided list at any given time, but not multiple members 
    simultaneously, as they all share the same storage.
*/

typedef union Token {
    char character;
    int digit;
} token_t;

typedef union Sensor {
    int t; // 4 bytes
    unsigned int c; // 8
    long double p; // 16
    long int h; // 8
} sensor_t;

int main() {
    token_t token = {.digit = 1};

    printf("digit: %d\n", token.digit);

    /*
        If we try to print the second attr of the union,
        it will get nothing, this is because all fields of a union
        sharethe same memory address.
        In our demo, setting digit as a integer, will overrites the 
        character. But trying to interpret the bytes of an int as a character will
        lead to an undefined behavior.
    */

    printf("character: %c\n", token.character);

    /*
        Now let's override our uniont with an actual character.
    */
    token.character = 'c';
    printf("character: %c\n", token.character);
    /*
    Trying to interpret a character as a digit
        it will return the ordinal ascii number of the stored character.
    */
    printf("digit: %d\n", token.digit);

    /* 
        remember, atributes of an union share the memory.
        keep in mind when designing unions, so programms doesn't 
        use unecessary memory for small data types, that whill
        be instantiated most of the time, leading to too much
        memory usage.
    */
    printf("%zu\n", sizeof(sensor_t));

    typedef union IntOrErr {
        int i;
        char err[256];
    } int_or_err_t;

    /*
        The following example show how ineficient
        int_or_err_t union is.
        Most of the time it is expected to store a int
        which is only 4 bytes long.
        But imagine you storing an array of 1.000.000 objects
        of type int_or_err.
        You will end up allocating 244.14 MB = (1.000.000 * 256 bytes ) of memory
    */
    //int_or_err_t big_array[1000000];
    /*
        the line above will lead to stack overflow during memory allocation.
        When dealing with too big memory allocation, it would be better to allocate
        memory on the heap, using malloc

    */

    // Calculate the total size needed
    size_t total_size = 1000000 * sizeof(int_or_err_t);

    // Allocate the memory on the heap
    int_or_err_t *big_array_ptr = (int_or_err_t *)malloc(total_size);

    printf("pointer to the heap %zu bytes\n", sizeof(big_array_ptr));
    printf("bytes stored in the heap %zu bytes \n", total_size);

    // Don't forget to free the memory when done
    free(big_array_ptr);
    

    /*
        An way to optimize this previous example is storig a pointer
        which requires only 8 bytes,

        You will end up allocating 8 MB = (1.000.000 * 8 bytes ) of memory
    */
    typedef union OptimizedIntOrErr {
        int i;
        char *err;
    } opt_int_or_err_t;
    opt_int_or_err_t opt_big_array[1000000];
    printf("stored in the stack %zu bytes\n", sizeof(opt_big_array));

    typedef union Color {
        struct  {
            __uint8_t r;
            __uint8_t g;
            __uint8_t b;
            __uint8_t a;
        } comp ; // 4 bytes
        __uint32_t rgba; // 4 bytes
    } color_t; // 4 bytes long

    /*
        The above union show another example of an efficient way to model
        a union. Taking ing consideration each 1 byte long of an uint
        and a 4 bytes long of a uint32
    */
    printf("%zu bytes long\n", sizeof(color_t));

    return 0;
}