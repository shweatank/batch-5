#include<stdio.h>
int main()
{
	int n,i;
	scanf("%d",&n);
	for(i=31;i>=0;i--)
	{
		printf("%d ",(n>>i)&1);
	}
	printf("\n");
	n=n&(n-1);
	
	for(i=31;i>=0;i--)
	{
		printf("%d ",(n>>i)&1);
	}
	printf("\n");
}
