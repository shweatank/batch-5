#include<stdio.h>
int main()
{
	int n;
	scanf("%d",&n);
	if(n&(n-1))
	{
		printf("not pow of 2\n");
	}
	else
	{
		printf("pow of 2\n");
	}
	return 0;
}
