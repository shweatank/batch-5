#include<stdio.h>
int main()
{
	int n;
	scanf("%d",&n);
	for(int i=31;i>=0;i--)printf("%d ",(n>>i)&1);
	printf("\n");
	n=((n&(0xAAAAAAAA))>>1)|((n&(0x55555555))<<1);
	
	for(int i=31;i>=0;i--)printf("%d ",(n>>i)&1);
	printf("\n");
}
