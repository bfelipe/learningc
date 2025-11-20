#include <stdio.h>

int main() {

    /*
        C is a strong typed language.
        This means it can't store data of a different
        value into a variable that is implicit declared with another type.
    */
    int x = 10;
    // float x = 1.23; raise a compile error can't change type    
    x = 1.23; // update x, but this will cast down to 1
    printf("x: %d\n", x);

   /*
    You can also define a variable which can't change its value
    using const
   */
    const int i = 1;
    // i = 2; error: assigment of read-only variable

}