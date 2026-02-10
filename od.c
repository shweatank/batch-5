#include <stdio.h>

int main() {
    int octal;
    int decimal = 0;
    int base = 1; // 8^0

    printf("Enter an octal number: ");
    scanf("%o", &octal);  // %o reads octal directly

    int temp = octal;

    while (temp > 0) {
        int lastDigit = temp % 10;
        decimal += lastDigit * base;
        base *= 8;
        temp /= 10;
    }

    printf("Decimal: %d\n", decimal);
    return 0;
}

