#include <stdio.h>

int main() {
    int octal;
    printf("Enter an octal number: ");
    scanf("%o", &octal);  // %o reads octal directly

    printf("Binary: ");

    // Print binary using bit manipulation
    int started = 0; // flag to skip leading zeros
    for (int i = sizeof(int) * 8 - 1; i >= 0; i--) {
        int bit = (octal >> i) & 1;
        if (bit) started = 1;
        if (started) printf("%d", bit);
    }

    // If the number is 0
    if (!started) printf("0");

    printf("\n");
    return 0;
}

