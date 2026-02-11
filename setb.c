#include <stdio.h>

int main() {
    int num = 10;    // 1010
    int pos = 1;

    if (num & (1 << pos))
        printf("Bit is SET\n");
    else
        printf("Bit is NOT SET\n");

    return 0;
}

