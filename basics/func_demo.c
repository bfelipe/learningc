#include <stdio.h>
#include "functions.h"

/*
    Important note about importing.
    There are three different ways to import modules.
    Libraries installed in the standard system (package manager) is imported like this:
    
    #include <module.h>

    Libraries located in specific uses double quote:

    #include "module.h"

    Libraries located in sub directories uses angle bracket:

    #include <folder/module.h>
*/


int main() {
    int result = sum(1, 2);
    printf("Sum result %d\n", result);
    
    char * name = get_name();
    say_hello(name);

    /*
    In this example we are trying to execute a program which a non-existed function hello().
    This will raise a implicit-function-declaration error with an exit code -1.
    The complier will do its best to show you possible errors in your code during compilation time.
    */
    // hello();
    return 0;
}