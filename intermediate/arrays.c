#include <stdio.h>
#include <stddef.h> // For size_t in sizeof output

/*
    ## 📚 C Array and Memory Fundamentals

    Arrays are a **fixed size, ordered collection** of a given type.
    Arrays are stored **contiguously in memory**, like structs.
    
    Element **access** (e.g., int_arr[i]) is an **O(1)** (constant time) operation, 
    as the compiler simply calculates the memory address from the base address and index.

    The size in bytes of an array can be found through:
    sizeof(array) = (number of elements) * sizeof(stored type).

    We access and modify data through **index offset** (starting at 0).
*/


int main() {
    
    // --- Basic Array Iteration ---
    int int_arr[5] = {0, 1, 2, 3, 4};

    for (int i = 0; i <= 4; i++) {
        printf("idx: %d = %d\n", i, int_arr[i]);
    }

    // --- Undefined Behavior (UB) and Memory Safety ---
    
    /*
        It is critical to be careful when dealing with arrays, as C does NOT perform 
        bounds checking. Accessing memory outside the array's boundary leads to 
        **Undefined Behavior (UB)**.

        **Segmentation Fault (Segfault)**: This is a program crash that happens when 
        your process tries to access a virtual memory address that the OS has not 
        reserved for your process or when it lacks read/write permission (caught by the MMU).

        **Buffer Overflow**: This happens when you **write** into memory past the 
        size reserved for an array, **overwriting adjacent data** on the stack or heap.
    */

    // Example of UB (Out-of-Bounds Read) - Might crash or read garbage data
    // int data = int_arr[9999]; 

    // --- String Literal Pitfall: Missing Null Terminator ---
    
    // These arrays are exactly the size of the characters, leaving NO space for '\0'.
    char engineer[6] = "Turing"; 
    char assistant[4] = "Joan";

    /* Look what happens when we print these non-terminated strings:
    */
    printf("\n--- UB Example (Out-of-Bounds Read) ---\n");
    printf("engineer: %s, %zu\n", engineer, sizeof(engineer));
    printf("assistant: %s, %zu\n", assistant, sizeof(assistant));
    /*
        The reason one of the variables printed extra characters 
        is due to **Undefined Behavior (UB)**. Strings require a **null terminator (\0)**, 
        which needs an extra byte.
        The **%s** operator reads from the first address and continues reading 
        sequential memory until it finds a **\0** byte.
    */
   
    // --- Buffer Overflow Example ---
    // The previous attempt to "fix" the string by manually adding '\0' was an error:
    
    // engineer[7] = '\0'; // Writes two bytes past array boundary (UB/Buffer Overflow)
    // assistant[5] = '\0'; // Writes two bytes past array boundary (UB/Buffer Overflow)
    
    /*
        Those lines lead to a **buffer overflow** by writing into memory outside 
        the array's reserved space, corrupting adjacent values on the stack.
    */

    // --- Proper Fix: Allocating Space for the Null Terminator ---
    printf("\n--- Corrected String Handling ---\n");
    /*
        The proper way to fix this issue is by creating the arrays with 1 extra byte
        to safely hold the null terminator.
    */
    char good_engineer[7] = "Turing"; 
    char good_assistant[5] = "Joan";
    
    printf("engineer: %s, %zu\n", good_engineer, sizeof(good_engineer));
    printf("assistant: %s, %zu\n", good_assistant, sizeof(good_assistant));
    /*
        Alternatively, omitting the size lets the compiler calculate it correctly (6+1=7 bytes).
        char easy_engineer[] = "Turing"; 
    */

    /*
        Arrays and pointers are close related.
        Arrays names points to the first element of the array.
        We can also use pointer aritmetic to access other elements 
        of the array.
    */

    int *int_arr_ptr = int_arr;
    int val_at_index = int_arr[2];
    int val_by_ptr = *(int_arr_ptr + 2);
    int *ptr_aritmetic = int_arr + 2;
    int val_by_ptr_aritmetic = *ptr_aritmetic;

    printf("%d, %d, %d\n", val_at_index, val_by_ptr, val_by_ptr_aritmetic);

    /*
        We can also access and manipulate data using pointer aritmetic in arrays
        of structs.
    */

    typedef struct Coordinate {
        int x;
        int y;
        int z;
    } coordinate_t;

    coordinate_t points[3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    printf("Coords at index 1: %d, %d, %d\n", points[1].x, points[1].y, points[1].z);

    coordinate_t *ptr = points;
    printf("Coords at index 1: %d, %d, %d\n", (ptr+1)->x, (ptr+1)->y, (ptr+1)->z);

    /*
        We can also cast our array of struct into an array of same type
    */

    int *ptr_array = (int *)points;
    for (int i = 0; i < 9; i++) {
        printf("idx: %d - %d\n", i, ptr_array[i]);
    }

    // array decay to pointer
    int short_arr[3] = {1, 2, 3};
    int *ptr_short_arr = short_arr;
    int val_short_arr = *(ptr_short_arr + 2);
    printf("%d\n", val_short_arr);
    
    return 0;
}