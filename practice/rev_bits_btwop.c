#include<stdio.h>
int main()
{
	int i=0,j=31,n,t;
	scanf("%d",&n);
	t=n;
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
	printf("using bitwise op\n");
	t|=(((t<<24)&0xFF000000)|((t>>24)&0xFF)|((t>>4)&0x0000FF00)|((t<<4)&0x00ff0000));
	for(i=31;i>=0;i--)
	{
		printf("%d ",(n>>i)&1);
	}
	printf("\n");
	return 0;
}
