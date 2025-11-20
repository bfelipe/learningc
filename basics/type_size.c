#include <stdio.h>
#include <stdbool.h>

/*
    The size of any fundamental type (like int, long, or float) can
    vary across different platforms, CPU architectures, and compilers.
    This variability is because the C standard only guarantees minimum
    sizes, allowing the implementation (the compiler/OS combination)
    to choose the most efficient size for the target machine (e.g., 32-bit vs. 64-bit).

    You can reliably find the size, in bytes, of any type or
    object using the sizeof operator.

    It is also beneficial when working with loops and data structures
    because the value it returns is of type size_t. Using size_t for
    counters and indices ensures the program can correctly handle the
    maximum possible size of any object on the platform, preventing
    potential integer overflow and out-of-range errors when dealing
    with very large arrays or memory blocks.
*/
int main() {
    int a = 10;
    /*
        The size_t type is a special type that is guaranteed to be able
        to represent the size of the largest possible object in the target
        platform's address space (i.e. can fit any single, non-struct value inside of it).
    */
    size_t of_a = sizeof(a);
    printf("the size of int %d is %zu bytes\n", a, of_a);

    /*
        Overall The size of each basic data type is measured as follow:
        keep in mind some may vary based on the machine architecture.
        
        char            1 byte
        short           2 bytes
        int             4 bytes
        long            8 bytes
        long long       8 bytes
        signed int      4 bytes
        unsigned int    4 bytes
        float           4 bytes
        double          8 bytes
        long double     16 bytes
        bool            1 byte
        struct          sum in bytes of each field
        pointer         8 bytes
        array of type   num of elements * size in bytes of that type
    */
    printf("the size of char          is %zu byte\n", sizeof(char));
    printf("the size of bool          is %zu byte\n", sizeof(bool));
    printf("the size of short         is %zu bytes\n", sizeof(short));
    printf("the size of int           is %zu bytes\n", sizeof(int));
    printf("the size of long          is %zu bytes\n", sizeof(long));
    printf("the size of long long     is %zu bytes\n", sizeof(long long));
    printf("the size of signed int    is %zu bytes\n", sizeof(signed int));
    printf("the size of unsigned int  is %zu bytes\n", sizeof(unsigned  ));
    printf("the size of float         is %zu bytes\n", sizeof(float));
    printf("the size of double        is %zu bytes\n", sizeof(double));
    printf("the size of long double   is %zu bytes\n", sizeof(long double));
    typedef struct {
        int a;
        float b;
        double c;
    } my_struct_t;
    printf("the size of struct        is %zu bytes\n", sizeof(my_struct_t));
    printf("the size of pointer       is %zu bytes\n", sizeof(char *));
    printf("the size of array of type is %zu bytes\n", sizeof(int [10]));

    return 0;
}