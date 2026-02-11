#include <stdio.h>

int main() {
    int num = 8;   // 1000
    int pos = 1;

    num = num | (1 << pos);

    printf("After setting bit: %d\n", num);

    return 0;
}

