#include <stdio.h>
#include <stdbool.h>

/*
    This example shows some of the basic types C programming language has.
*/

int main() {

    // --- Integer Types ---
    
    // char: Single character value. Use single quotes.
    char c = 'Z'; 
    
    // short: Small integer.
    short s = 32000; 
    
    // int: Standard integer, default size.
    int i = -98765; 
    
    // long: Large integer. Append 'L' to constants for best practice.
    long l = 4000000000L; 
    
    // long long: Very large integer (at least 64-bit). Append 'LL'.
    long long ll = 987654321098765432LL; 

    // signed int: Same as 'int', explicitly allows negative and positive.
    signed int si = -54321; 
    
    // unsigned int: Only positive values, doubles the positive range. Append 'U'.
    unsigned int ui = 4000000000U; 

    // --- Floating-Point Types ---
    
    // float: Single-precision. Append 'f' to constant.
    float f = 123.456789f; 
    
    // double: Double-precision (standard for most real numbers).
    double d = 123456789.01234567; 
    
    // long double: Extended precision. Append 'L'.
    long double ld = 123456789.012345678901234L; 

    // --- Boolean Types ---
    // Note: Booleans are printed as integers (1 for true, 0 for false).
    _Bool _b = 0; // basic boolean type c(99), initialized to false
    bool b = 1; // alias for _Bool, initialized to true
    bool t = true; // true is a macro for 1
    bool bf = false; // false is a macro for 0

    // --- Character Array (String) ---
    // Pointer to the first character of a string literal.
    char * name = "Alan Turing";

    // --- Corrected Print Statements ---

    // Integer Prints
    // %c for char as character, %hd for short, %ld for long, %lld for long long
    printf("Example of char: %c\n", c); 
    printf("Example of short: %hd\n", s);
    printf("Example of int: %d\n", i);
    printf("Example of long: %ld\n", l);
    printf("Example of long long: %lld\n", ll);
    printf("Example of signed int: %d\n", si);
    printf("Example of unsigned int: %u\n", ui); // %u for unsigned

    // Float Prints
    // %f for float and double, %Lf for long double
    printf("Example of float: %.6f\n", f); // Printing 6 digits of precision
    printf("Example of double: %.15f\n", d); // Printing 15 digits of precision
    printf("Example of long double: %.18Lf\n", ld); // Printing 18 digits of precision

    // Boolean and String Prints
    // %d for bool, %s for string
    printf("Example of _Bool (0): %d\n", _b);
    printf("Example of bool (1): %d\n", b);
    printf("Example of bool (true/1): %d\n", t);
    printf("Example of bool (false/0): %d\n", bf);
    printf("Example of char array (string): %s\n", name);


    // You can also cast a type to another
    int my_int_type = 1;
    float my_casted_int = (float)my_int_type;
    printf("Cast in to float %f\n", my_casted_int);
    return 0;
}