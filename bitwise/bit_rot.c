#include<stdio.h>

/*
    Function: bit_rot
    Description:
    Performs right rotation of bits of integer x by n positions.
    
    Steps:
    1. Extract the last n bits of x.
    2. Right shift x by n positions.
    3. Move the extracted bits to the MSB side.
    4. Combine them with OR operation.
*/

int bit_rot(int x,int n)
{
    // Extract last n bits of x
    int temp = ((1<<n) - 1) & x;

    // Shift x right by n positions
    x >>= n;

    // Move extracted bits to the leftmost positions
    x |= (temp << (32-n));

    return x;
}


/*
    Function: print_binary
    Description:
    Prints the binary representation of a 32-bit integer.
*/

void print_binary(int n)
{
    // Loop through all 32 bits from MSB to LSB
    for(int i = 31; i >= 0; i--)
    {
        // Check if the ith bit is set
        if(n & (1<<i))
            printf("1");
        else
            printf("0");
    }

    printf("\n");
}


/*
    Main Function
    Takes input from user and performs bit rotation.
*/

int main()
{
    int x,n;

    printf("Enter the number: ");
    scanf("%d",&x);

    printf("Enter the number n: ");
    scanf("%d",&n);

    printf("before rotate: ");
    print_binary(x);

    // Perform right rotation
    x = bit_rot(x,n);

    printf("after rotate: ");
    print_binary(x);

    return 0;
}
