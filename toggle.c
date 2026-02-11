#include <stdio.h>

int main() {
    int num = 29;
    int count = 0;

    while (num) {
        count += num & 1;
        num >>= 1;
    }

    printf("Number of set bits: %d\n", count);

    return 0;
}

