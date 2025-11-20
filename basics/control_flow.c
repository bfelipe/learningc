#include <stdio.h>

int main() {
    int score = 10;
    if (score < 5) {
        printf("bad\n");
    } else if (score < 10) {
        printf("good\n");
    } else {
        printf("excelent\n");
    }

    // C also provide a ternary operator
    int a = 5, b = 10;
    // it reads if a > b do a else b
    int max = a > b ? a : b;
    printf("max: %d\n", max);

    /*
        Another useful control flow provided by c is
        the switch.
        It allow us to specificaly trigger a path
        based on the given argument.
        Remember to add a return or break, and a default
        case, otherwise you can fall into undesired effects.
    */
    switch (score) {
        case 5:
            printf("bad\n");
            break;
        case 6 | 7:
            printf("good\n");
            break;
        case 8 | 9:
            printf("great\n");
            break;
        case 10:
            printf("excelent\n");
            break;
        default:
            printf("unkown\n");
            break;
    }
    return 0;
}