#include <stdio.h>

int main() {
    int a = 5, b = 2;
    float result1, result2;

    // Implicit type conversion
    result1 = a / b;   // Both are integers, so result is integer division
    printf("Implicit conversion (without casting): %f\n", result1);

    // Explicit type conversion
    result2 = (float)a / b;  // 'a' is explicitly converted to float
    printf("Explicit conversion (with casting): %f\n", result2);

    return 0;
}

