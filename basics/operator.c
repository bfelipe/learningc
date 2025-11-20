#include <stdio.h>

/*
    C provies all mathematical operations: +, -, * and /

    In situations where floor and power operations are required, you can
    use the floor() and pow() function from math.h module.

*/

int main() {

    int a = 5;
    int b = a++; // b becomes 5 then a becomes 6
    printf("a: %d b: %d\n", a, b);

    int c = 5;
    int d = ++c; // d becomes 6 then c becomes 6
    printf("c: %d, d: %d\n", c, d);
    return 0;
}