#include <stdio.h>

/*
    Structs are components to arrange memory
    in a efficient way into memory, and move data
    across your program.

    You can define a struct in two different ways
*/

struct NameOfStruct {
    int x;
    int y;
};

// Or giving an alias to the struct
typedef struct {
    int x;
    int y;
} name_of_struct_t;

// Using the second approach, you should end with a _t as convention

/*
    Structs arrange data in a contiguous block of memory.
    Each field is placed in this block according to alignment rules.
    We call this the memory layout of the struct.

    As a guideline, it is good practice to order fields from largest
    to smallest type. This can help reduce padding and make the
    struct more compact.

    Whenever we define a struct, the compiler may insert padding bytes
    so that the CPU can access each field efficiently and maintain
    proper alignment.
*/
typedef struct {
  char a; // 1 byte
  // padding 7 bytes
  double b; // 8 bytes
  char c; // 1 byte
  char d; // 1 byte
  // padding 6 bytes
  long *e; // 8 bytes
  char f; // 1 byte
  // padding 7 bytes
} poorly_aligned_t; // total 40 bytes long

typedef struct {
  double b; // 8 bytes
  long *e; // 8 bytes
  char a; // 1 byte
  char c; // 1 byte
  char d; // 1 byte
  char f; // 1 byte
  // padding 4 bytes
} better_t; // total 24 bytes long

int main() {

    /*
        There are different ways to initialize a struct
    */
   // Zero initilizer
    struct NameOfStruct ns = {0}; // set all fields with zero value;
    // Positional initializer
    struct NameOfStruct nns = {0, 0};
     // Designated initializer
    name_of_struct_t nst = {
        // this method requires you to use . before each field name
        .x = 0,
        .y = 0
    };
    // One important thing to notice it that you don't need to repeat
    // struct keywork by using the alias struct definition

    printf("Memory address of struct: %p\n", &nst);
    printf("Bytes of bad struct layout %zu\n", sizeof(poorly_aligned_t));
    printf("Bytes of good struct layout %zu\n", sizeof(better_t));
    return 0;
}