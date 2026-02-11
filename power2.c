#include <stdio.h>

int main() {
    int num = 16;

    if (num && !(num & (num - 1)))
        printf("Power of 2\n");
    else
        printf("Not Power of 2\n");

    return 0;
}

