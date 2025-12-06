#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define variables using a type that overflows easily (unsigned char, max 255)
typedef unsigned char u_char;

#define LARGE_INPUT_COUNT 200 // The number of times we *think* we are copying data

int main() {
    printf("--- Simple Integer Overflow Buffer Overflow Example ---\n\n");

    // 1. Integer Overflow Setup
    // We want to calculate 200 * 2 (which is 400).
    // But we store the result in an unsigned char, which can only hold up to 255.
    
    u_char count = LARGE_INPUT_COUNT; // 200
    u_char element_size = 2;          // 2 bytes per element (just an arbitrary small size)
    u_char total_size_overflowed;

    // --- The Calculation Error ---
    total_size_overflowed = count * element_size; 
    
    // 200 * 2 = 400. 
    // On an 8-bit unsigned char, 400 wraps around: 400 - 256 = 144.
    
    printf("Intended size: 400 bytes.\n");
    printf("Overflowed size (actual memory allocated): %hhu bytes\n", total_size_overflowed);

    // 2. Memory Allocation
    // malloc is called with the small, wrapped-around size (144 bytes).
    char *buffer = (char *)malloc(total_size_overflowed);

    if (buffer == NULL) {
        printf("Allocation failed.\n");
        return 1;
    }

    // 3. Buffer Overflow
    // The program logic *thinks* the buffer size is large (400 bytes) 
    // because it assumes the calculation succeeded without overflow.
    
    // We will attempt to write 200 bytes, which is greater than 144 bytes.
    printf("Attempting to write 200 bytes into the buffer (size 144)...\n");

    // The destination address is 'buffer', but the size we use (200) exceeds the 
    // allocated size (144).
    memset(buffer, 'Z', 200); 

    // The first 144 'Z's are safe. The subsequent 56 'Z's are written past the 
    // allocated end of the buffer, causing a **Buffer Overflow**.
    printf("Buffer Overflow occurred: 56 bytes written outside allocated memory.\n");

    free(buffer);
    return 0;
}