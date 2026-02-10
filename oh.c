#include <stdio.h>

int main() {
    int octal;
    int decimal = 0;
    int base = 1;
    char hex[20];
    int index = 0;

    printf("Enter an octal number: ");
    scanf("%d", &octal);

    // Octal to Decimal
    while (octal != 0) {
        int digit = octal % 10;
        decimal += digit * base;
        base *= 8;
        octal /= 10;
    }

    // Special case
    if (decimal == 0) {
        printf("Hexadecimal: 0\n");
        return 0;
    }

    // Decimal to Hexadecimal
    while (decimal != 0) {
        int rem = decimal % 16;

        if (rem < 10)
            hex[index++] = rem + '0';
        else
            hex[index++] = rem - 10 + 'A';

        decimal /= 16;
    }

    printf("Hexadecimal: ");
    for (int i = index - 1; i >= 0; i--) {
        printf("%c", hex[i]);
    }

    printf("\n");
    return 0;
}

