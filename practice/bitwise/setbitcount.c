#include<stdio.h>
void main()
{
	int n,c=0,pos;
	printf("enter the numberr..\n");
	scanf("%d",&n);
        for(pos=31;pos>=0;pos--)
		printf("%d ",n>>pos&1);
	while(n)
	{
		n=n&(n-1);
		c++;

	}
	printf("\ncount:%d",c);
}
