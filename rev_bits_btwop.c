#include<stdio.h>
int main()
{
	int i=0,j=31,n;
	scanf("%d",&n);
	for(i=31;i>=0;i--)
	{
		printf("%d ",(n>>i)&1);
	}
	i=0;
	printf("\n");
	for(;i<j;i++,j--)
	{
		if(((n>>i)&1)!=((n>>j)&1))
		{
			n^=1<<i;
			n^=1<<j;
		}
	}
	for(i=31;i>=0;i--)
	{
		printf("%d ",(n>>i)&1);
	}
	printf("\n");
	return 0;
}
