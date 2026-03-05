#include<stdio.h>

/*
    Function: print_binary
    Description:
    Prints the 32-bit binary representation of an integer.
    It checks each bit from MSB (31) to LSB (0).
*/
void print_binary(int n)
{
    // Loop through all 32 bits from MSB to LSB
    for(int i = 31; i >= 0; i--)
    {
        // Check whether the i-th bit is set
        if(n & (1<<i))
            printf("1");
        else
            printf("0");
    }

    printf("\n");
}


/*
    Function: fun_set_bit
    Description:
    Replaces n bits in x at position p with the rightmost
    n bits from y.

    Steps:
    1. Extract the rightmost n bits from y.
    2. Clear n bits in x starting at position p.
    3. Insert the extracted bits into x at position p.
*/
int fun_set_bit(int x,int n,int p,int y)
{
    // Extract the last n bits from y
    int temp = y & ((1<<n) - 1);

    // Clear n bits in x starting at position p
    x = x & ~(((1<<n) - 1) << (p - n + 1));

    // Insert extracted bits into x
    x |= temp << (p - n + 1);

    return x;
}


/*
    Main Function
    Takes input values and demonstrates the bit replacement.
*/
int main()
{
    int x,y,n,p;

    printf("Enter the value x: ");
    scanf("%d",&x);

    printf("Enter the value y: ");
    scanf("%d",&y);

    printf("Enter the value n: ");
    scanf("%d",&n);

    printf("Enter the value p: ");
    scanf("%d",&p);

    // Print binary value of x before modification
    printf("before fun_set x: ");
    print_binary(x);

    // Print binary value of y
    printf("before fun_set y: ");
    print_binary(y);

    printf("Position : %d\n",p);
    printf("N bits : %d\n",n);

    // Replace bits
    x = fun_set_bit(x,n,p,y);

    // Print modified x
    printf("after fun_set x: ");
    print_binary(x);

    return 0;
}
