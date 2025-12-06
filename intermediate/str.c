#include <stdio.h>
#include <string.h>
/*
    Strings is text representation
    in a program.
    We can define a string using an array
    of char.
    Strings are simple, it doesn't store its length.
    The length of a string is determined by the found of
    null-terminator \0 at the end.
    String manipulation is done using pointers, until it found
    a null-terminator.

    There are dedicated string functions in the string.h module.
*/

void concat(char *dst, char *src) {
    /*
        Let's study how pointers can be
        useful with string manipulation.
    */
    int dst_off = 0, src_off = 0;
    while (dst[dst_off] != '\0') {
        dst_off += 1;
    }

    while (src[src_off] != '\0') {
        dst[dst_off] = src[src_off];
        dst_off += 1;
        src_off += 1;
    }

    dst[dst_off] = '\0';
}

void concat_ptr(char *dst, char* src) {
    while (*dst != '\0') {
        dst += 1;
    }
    while (*src != '\0') {
        *dst = *src;
        dst += 1;
        src += 1;
    }

    *dst = '\0';
}

int main() {

    char *str = "Alan Turing";
    char str_arr[12] = "Alan Turing"; // there is one extra byte to hold the \0 terminator

    printf("String: %s, %zu\n", str, strlen(str));
    printf("String: %s, %zu\n", str_arr, strlen(str_arr));

    /*
        strlen function determines the length of the string
        until it found null-terminator
    */

    /*
        strcat function allow you to concatenate two strings.
        It will find the location of the null-terminator, and starts
        copying each character of second string, at the end of the first
        starting from the null-terminator.
    */
    // printf("%zu\n", sizeof("Alan Turing the father of computing"));
    char new_str[36];
    strcpy(new_str, str); // copy content of str into new_str
    strcat(new_str, " the father of computing");
    printf("String: %s, byte_size: %zu, len: %zu\n", new_str, sizeof(new_str), strlen(new_str));

    /*
        While strcat function allow us to contatenate an string,
        it also can lead to a buffer overflow. Corrupting the next portions 
        of memory.
        A safer approach is by using strncat, where you can pass a third
        parameter as the number of characters to be copied.

        size_t remaining_space = sizeof(new_str) - strlen(new_str) - 1;
        strncat(new_str, " the father of computing", remainging_space); 
    */

    /*
        Using a custom string concatenation function.
        However it works as a simple strcat, which means
        the developer should guarantee the dst has
        enougth space to store the src string.
    */
    char dst[36] = "Alan Turing";
    concat_ptr(dst, " the father of computing");
    printf("String: %s, byte_size: %zu, len: %zu\n", dst, sizeof(dst), strlen(dst));

    /*
        The string module also incluse other functions
        strcat concat one string with another
        strncat concact certain number of characters from one string to another
        strcpy make a copy
        strncpy make a copy of certain number of characters
        strlen return the len of string until it find \0
        strchr return a pointer to the first occurence of a char
        strstr return a pointer to the first occurence of a substring
        strcmp compare two strings lexicographically
            -1 if less - hello < world
            0  if equal - hello == hello
            1  if higher - world > hello
    */

    return 0;
}