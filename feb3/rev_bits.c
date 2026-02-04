#include<stdio.h>

void binary(int num)
{
	int i;
	for(i=31;i>=0;i--)
		((num>>i)&1) ? printf("%d ",1): printf("%d ",0);
	printf("\n");
}

void rev(int *num)
{
	int i,j;
	for(i=31,j=0;i>j;i--,j++)
	{
		if(((*num>>i)&1)!=((*num>>j)&1))
		{
			*num^=1<<i;
			*num^=1<<j;
		}
	}
}

int main()
{
	int num=10;
	binary(num);
	rev(&num);
	binary(num);
}
