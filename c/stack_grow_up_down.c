#include <stdio.h>

int main() {
    char x, y;
    printf("Address of x: %p\n", &x);
    printf("Address of y: %p\n", &y);
    if ((&y - &x) < 0) {
        printf("Down stack\n");
    } else {
        printf("Up stack\n");
    }
    return 0;
}

