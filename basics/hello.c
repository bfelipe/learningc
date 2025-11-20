#include <stdio.h>

/*
    This is our first c program.
    C is a low level systems programming language.
    This is a simple example of a hello world programm.

    At the beginning of our file we declare our header with include.
    Headers are proxies to the implementation functions and structs 
    across our project.
    In this example we are using stdio.h which stands for standard input/output.
    It allows us to make operations such as printing data into stdout file in the
    operational system.
    printf function stands for print formatted.

    Bellow you can see a main function which serves as the entry point of our program.
    Take a look it returns int. In c we usually define 0 as success and 1 so signal failure.
    So once our program is successfully executed it returns the signal 0 and exit.

    Note each line we declare an operation is ended with ; token. This tells the compiler we reached
    and end of line. Without it the compile will raise an error.
    
    Note this is an example of a multiline comment


    To compile our code you can use any of the following example in your terminal.

    gcc hello.c

    This will create a binary named a.out

    or 

    gcc hello.c -o hello

    The argument -o allows you to name the compiled binary with any given name. This example just hello.
*/

int main() {
    // this is an example of a single line comment
    printf("Hello World!\n");
    return 0;
}