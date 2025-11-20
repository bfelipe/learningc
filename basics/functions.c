#include <stdio.h>
#include "functions.h"

/*
    A simple function declaration can be structured as follow:

    return_type function_name(arg_type arg_name) {}
*/
int sum(int a, int b) {
    return a + b;
}

/*
    A function can be declared in such way it doesn't return
    anything, in this case you pass a void keyword instead of
    a return type
*/
void say_hello(char * name) {
    printf("hello %s\n", name);
}

/*
    Functions also don't need to take any argument, and it may or
    may note return something. In situations like this you pass
    void as argument, and the return type or void at the start of
    function definition
*/
char * get_name(void) {
    return "Alan Turing";
}