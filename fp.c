#include <stdio.h>

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int div(int a, int b) { return a / b; }

int main()
{
    int a, b, choice;
    int (*fp)(int, int);

    printf("Enter the values of a and b: ");
    scanf("%d %d", &a, &b);

    printf("1.ADD  2.SUB  3.MUL  4.DIV\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            fp = add;
            break;

        case 2:
            fp = sub;
            break;

        case 3:
            fp = mul;
            break;

        case 4:
            fp = div;
            break;

        default:
            printf("Invalid choice\n");
            return 0;
    }

    printf("Result is %d\n", fp(a, b));
    return 0;
}

