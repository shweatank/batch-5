#include <stdio.h>
#include <string.h>

int main() {
    char binary[65]; // Supports up to 64-bit binary input
    printf("Enter a binary number: ");
    scanf("%s", binary);

    int len = strlen(binary);
    int remainder = len % 3; // For padding

    printf("Octal: ");

    // Print the first group if it's not full (1 or 2 bits)
    if (remainder != 0) {
        int val = 0;
        for (int i = 0; i < remainder; i++) {
            val = val * 2 + (binary[i] - '0');
        }
        printf("%d", val);
    }

    // Process remaining full groups of 3
    for (int i = remainder; i < len; i += 3) {
        int val = 0;
        for (int j = 0; j < 3; j++) {
            val = val * 2 + (binary[i + j] - '0');
        }
        printf("%d", val);
    }

    printf("\n");
    return 0;
}

