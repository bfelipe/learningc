#include <stdio.h>

int main() {

    int start = 0, end = 10;
    // For loop
    for (; start < end; start++) {
        printf("%d\n", start);
    }
    // While loop
    while (start > 0) {
        printf("%d\n", start);
        start--;
    }
    // Do while loop
    do {
        printf("%d\n", start);
        start++;
    } while (start <= end);

    return 0;
}