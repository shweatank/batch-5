#include<stdio.h>
int main()
{
	int i=0,rev=0,n,t;
	scanf("%d",&n);t=n;
	for(i=31;i>=0;i--)
	{
		printf("%d ",(n>>i)&1);
	}
	printf("\n");
	i=0;
	n=(n>>16)|(n<<16);
	 n = ((n & 0xFF00FF00U) >> 8) | ((n & 0x00FF00FFU) << 8);
    	n = ((n & 0xF0F0F0F0U) >> 4) | ((n & 0x0F0F0F0FU) << 4);
	    n = ((n & 0xCCCCCCCCU) >> 2) | ((n & 0x33333333U) << 2);
	    n = ((n & 0xAAAAAAAAU) >> 1) | ((n & 0x55555555U) << 1);
	for(i=31;i>=0;i--)
	{
		printf("%d ",(n>>i)&1);
	}
	printf("\n");
	t|=((t<<24) & 0xff000000)|((t>>24) & 0x000000ff)|((t>>4) & 0x0000ff00)|((t<<4) &0x00ff0000);
	
	for(i=31;i>=0;i--)
	{
		printf("%d ",(t>>i)&1);
	}
	return 0;
}
