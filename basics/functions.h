/*
    Files with an .h end means header.
    Headers works as proxies to concrete
    implementation of types and functions.
    Think of it as an interface or contract, where you
    define the signature of the functions.

    Sometimes our headers can be imported many times in our program.
    To avoid redefinition errors due to multiple import we can use
    pragma once, it will tell the compile to only include this file once.

    An alternative is using header guards, instead of using pragma once
    we can use

    #ifndef MY_HEADER_MACRO
    #define MY_HEADER_MACRO
    ...
    #endif

    However this second approach can be error prone. So using pragma is a better tool
    for this situation.
*/
#pragma once

int sum(int a, int b);

void say_hello(char * name);

char * get_name(void);