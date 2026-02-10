#include <stdio.h>

/* Function declarations */
int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int divi(int a, int b) { return a / b; }

int main()
{
    int choice, x, y, result;

    /* Function pointer declaration */
    int (*fp)(int, int);

    printf("Enter two numbers: ");
    scanf("%d %d", &x, &y);

    printf("Choose operation:\n");
    printf("1. Addition\n2. Subtraction\n3. Multiplication\n4. Division\n");
    scanf("%d", &choice);

    /* Assign function pointer based on choice */
    switch(choice)
    {
        case 1: fp = add; break;
        case 2: fp = sub; break;
        case 3: fp = mul; break;
        case 4: fp = divi; break;
        default: 
            printf("Invalid choice!\n");
            return 0;
    }

    /* Call function using pointer */
    result = fp(x, y);

    printf("Result = %d\n", result);

    return 0;
}

