#include<stdio.h>
int main()
{
	int n,x=0;
	scanf("%d",&n);
	x=n;
	for(int i=31;i>=0;i--)printf("%d ",(n>>i)&1);
	printf("\n");
	printf("adjbits\n");
	n=((n&0xAAAAAAAA)>>1)|((n&0x55555555)<<1);
	for(int i=31;i>=0;i--)printf("%d ",(n>>i)&1);
	printf("\n");
	printf("adj pairs\n");
	n=x;
	n=((n&0xCCCCCCCC)>>2)|((n&0x33333333)<<2);
	for(int i=31;i>=0;i--)printf("%d ",(n>>i)&1);
	printf("\n");
	printf("nibbles\n");
	n=x;
	n=((n&0xF0F0F0F0)>>4)|((n&0x0F0F0F0F)<<4);
	for(int i=31;i>=0;i--)printf("%d ",(n>>i)&1);
	printf("\n");
	printf("bytes\n");
	n=x;
	n=((n&0xFF00FF00)>>8)|((n&0x00FF00FF)<<8);
	for(int i=31;i>=0;i--)printf("%d ",(n>>i)&1);
	printf("\n");
	printf("half word...16-bits\n");
	n=x;
	n=((n&0xFFFF0000)>>16)|((n&0x0000FFFF)<<16);
	for(int i=31;i>=0;i--)printf("%d ",(n>>i)&1);
	printf("\n");
}
