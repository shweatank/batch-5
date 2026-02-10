#include <stdio.h>

int main() {
    int decimal;
    int octal[20];  // To store octal digits
    int index = 0;

    printf("Enter a decimal number: ");
    scanf("%d", &decimal);

    // Special case if decimal is 0
    if (decimal == 0) {
        printf("Octal: 0\n");
        return 0;
    }

    // Decimal → Octal
    while (decimal != 0) {
        octal[index++] = decimal % 8;
        decimal /= 8;
    }

    printf("Octal: ");
    // Print octal digits in reverse order
    for (int i = index - 1; i >= 0; i--) {
        printf("%d", octal[i]);
    }
    printf("\n");

    return 0;
}

