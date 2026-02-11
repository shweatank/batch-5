#include <stdio.h>

int main() {
    int num = 10;   // 1010
    int pos = 1;

    num = num & ~(1 << pos);

    printf("After clearing bit: %d\n", num);

    return 0;
}

