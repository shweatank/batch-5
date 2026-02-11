#include <stdio.h>

int main() {
    int a = 10;   // 1010
    int b = 6;    // 0110

    printf("AND  (a & b)  = %d\n", a & b);
    printf("OR   (a | b)  = %d\n", a | b);
    printf("XOR  (a ^ b)  = %d\n", a ^ b);
    printf("NOT  (~a)     = %d\n", ~a);

    return 0;
}

