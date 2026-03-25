#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n;

    // Ask the user for the number of Fibonacci terms
    printf("Enter the number of terms: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Please enter a positive integer.\n");
        return 1;
    }

    // Use dynamic memory allocation to avoid stack overflow for large n
    unsigned long long *ar = malloc(sizeof(unsigned long long) * n);
    if (!ar) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Initialize first two Fibonacci numbers
    unsigned long long a = 0, b = 1, next = 0;

    // Generate the Fibonacci sequence
    for(int i = 0; i < n; i++)
    {
        ar[i] = a;       // Store the current Fibonacci number
        next = a + b;    // Calculate the next Fibonacci number
        a = b;           // Update a
        b = next;        // Update b
    }

    // Print the generated Fibonacci sequence
    printf("Fibonacci sequence: ");
    for(int i = 0; i < n; i++)
    {
        printf("%llu ", ar[i]);
    }
    printf("\n");

    free(ar); // Free allocated memory
    return 0;
}
