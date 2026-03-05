#include<stdio.h>

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

int bit_inv(int x,int n,int p)
{
	int temp= (((1<<n)-1)<<(p-n+1)) ;

	x^=temp;

	return x;
}


int main()
{
	int x,n,p;
	printf("Enter the value x:");
	scanf("%d",&x);
	printf("Enter the value n:");
	scanf("%d",&n);
	printf("Enter the value p:");
	scanf("%d",&p);
	printf("before bit_inv x:");
	print_binary(x);
	printf("Position :%d\n",p);
	printf("N bits:%d\n",n);
	x=bit_inv(x,n,p);
	printf("after fun_set x:");
	print_binary(x);
	return 0;
}
