#include <stdio.h>

int main() {
    char hex[20];
    int decimal = 0;
    int i = 0;

    printf("Enter a hexadecimal number: ");
    scanf("%s", hex);

    while (hex[i] != '\0') {
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

        i++;
    }

    printf("Decimal: %d\n", decimal);
    return 0;
}

