#include <stdio.h>

/*
    Before jumping into pointers, we should learn how memory works and how it is designed.
    
    We can understand memory as a big array.

    Usually when your program runs, it becomes a process. In case it us running into a machine
    which is operated by an operational system, the OS will provide a virtual memory to your process.
    Which allow our process to safely isolate specific chunks of memory for itself. Withoug sharing it
    with other processes, which could potentially lead to various issues.
    This is different into embedded hardware which doesn't have an OS. You then have access to the
    physical memory of that hardware.

    Everytime we define a variable of a given type, we will layout into the virtual memory a range
    of bytes based on the size of that type.
    We then link out variable name to the first offset of that range.
    The memory address itself is a numerical value that is typically represented (or displayed)
    as a hexadecimal number (e.g., 0x7ffee...)

    Example.

    int age = 41;
    char * name = "Turing";

    Our program will require 12 bytes to store the two variables (age and name) themselves,
    plus 7 bytes stored elsewhere in memory for the string literal "Turing\0", for 
    a total of 19 bytes of memory used by the program.

    Keep in mind that our name variable is a pointer, which whill only store the first address
    of memory(elsewhere) that beggins our actuall string.
    Also, look at our string. Every string ends with a null terminator \0. 

    variable | value         | address | size
    age      | 41            | 0       | 4 bytes(offset 0-3)
    * name   | address 16    | 4       | 8 bytes(offset 4-11)
             |'T'            | 16      | 1 byte
             |'u'            | 17      | 1 byte
             |'r'            | 18      | 1 byte
             |'i'            | 19      | 1 byte
             |'n'            | 20      | 1 byte
             |'g'            | 21      | 1 byte
             |'\0'           | 22      | 1 byte
    
    Everytime our programm uses a variable, it will look into the address which our variable is pointing to.
    The variable name acts as a label for the first byte's address of the memory range allocated for that variable's data type.


    Now we can start out studies of pointers.
    Pointers are simply variables that stores memory addresses. Because its point to an address. Simple right?

    */

typedef struct {
        int x;
        int y;
} coordinates_t;

void update_x(coordinates_t t, int x) {
    printf("Address of copy %p\n", &t);    
    t.x = x;
}

coordinates_t update_t_x(coordinates_t t, int x) {
    printf("Address of copy %p\n", &t);
    t.x = x;
    return t;
}

int main() {
    // To get the memory address we uses address of operator &.
    int age = 41;
    int * address_of_variable_age = &age; // or ptr_of_variable_age
    printf("Address in memory of variable age %p\n", address_of_variable_age);

    /*
        Take a look into update_variable.py.
        We can update the variable of that instance by passing it to a function.
        However we can't do that in C.
        When we pass a struct to a variable we only passing a copy of it.
        To persist that update we should also return that copy.
    */

    coordinates_t t = {1, 2};
    printf("Address of original t %p\n", &t);
    update_x(t, 3);
    printf("t.x = %d\n", t.x); // didn't changed
    t = update_t_x(t, 3); // overrides the content in the address of t with the content of returned value
    printf("t.x = %d\n", t.x);
    printf("Address of new t %p\n", &t);

    /*
        Keep in mind, pointers are read-only.
        Often when working with pointers
        we are not interested in the address which the pointer points to.
        But instead we want its value.
        We can get the value which the pointer is pointing too using de-reference *.
        We can also use de-reference to update that value using * at the beginning of
        the signing value
    */
    int year = 1994;
    int * pointer = &year;
    int value_at_address = * pointer; // copy value pointed by the pointer
    printf("Year %d\n", year);
    printf("Pointer %p\n", pointer);
    printf("Value at address %d\n", value_at_address); // de-reference
    // update variable the pointer is pointing to using de-reference
    *pointer = 1995;
    printf("Updated year %d\n", year);
    printf("Value at address %d\n", value_at_address); // value didn't change

    /*
        When using pointers to struct, instead of accessing fields with struct.field we
        should use instead ->
    */

    coordinates_t example = {1, 2};
    coordinates_t * ptr_example = &example;
    printf("Struct pointer to field %d\n", ptr_example->x);
    // an alternative can be done like this (*ptr_example).x but its more verbose
    // keep in mind the . operator have higher precedence than *

    /*
        Pointer aritmetic is a technique that allow you to move the pointed
        pointer to the next offset
    */
    char *name = "Alan Turing";
    while (*name != '\0') {
        char c = *name;
        printf("%c\n", c);
        name += 1; // move the pointer to the next sequence in the memory layout
    }

    /*
        Finally, there is a special pointer that is defined as null-pointer
        It is a pointer that points to a zero value address in the memory layout
    */
    char *none = NULL;
    return 0;
}