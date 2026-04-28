#include<stdio.h>
#include<stdlib.h>

int toggle(int x,int s,int e)
{
/*	int i,j;
	for(i=s,j=e;i<j;i++,j--)
	{
		x=^(1<<i);
		x=^(1<<j);
	}*/

	int temp;
	temp=(((1<<(e-s+1))-1)<<s);
	return x^temp;
}

int main()
{
	printf("%d\n",toggle(-1,0,31));
}
