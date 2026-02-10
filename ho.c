#include <stdio.h>

int main() {
    char hex[20];
    int decimal = 0;
    int octal[20];
    int index = 0;

    printf("Enter a hexadecimal number: ");
    scanf("%s", hex);

    // Hexadecimal → Decimal
    for (int i = 0; hex[i] != '\0'; i++) {
        char ch = hex[i];
        decimal = decimal * 16;

        if (ch >= '0' && ch <= '9')
            decimal += ch - '0';
        else if (ch >= 'A' && ch <= 'F')
            decimal += ch - 'A' + 10;
        else if (ch >= 'a' && ch <= 'f')
            decimal += ch - 'a' + 10;
        else {
            printf("Invalid hexadecimal digit!\n");
            return 0;
        }
    }

    // Decimal → Octal
    if (decimal == 0) {
        printf("Octal: 0\n");
        return 0;
    }

    while (decimal != 0) {
        octal[index++] = decimal % 8;
        decimal /= 8;
    }

     printf("Octal: ");
    for (int i = index - 1; i >= 0; i--) {
        printf("%d", octal[i]);
    }
    printf("\n");

    return 0;
}
