#include<stdio.h>
int main()
{
	int n,t;
	scanf("%d",&n);t=n;
	for(int i=31;i>=0;i--)
		printf("%d ",(n>>i)&1);
	printf("\n");
	n|=0xAAAAAAAA;
	
	for(int i=31;i>=0;i--)
		printf("%d ",(n>>i)&1);
	printf("\n");
	n=t;
	n|=0x55555555;
	for(int i=31;i>=0;i--)
		printf("%d ",(n>>i)&1);
	printf("\n");
	return 0;
}
