#include <stdio.h>
#include <stdbool.h>

int main() {
    bool is_bigger = 1 < 2;
    if (is_bigger == true) {
        printf("true");
    } else {
        printf("false");
    }
    return 0;
}