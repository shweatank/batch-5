#include<stdio.h>
int main()
{
	int n;
	scanf("%d",&n);
	if((n>>31)&1)
	{
		printf("neg\n");
	}
	else
	{
		printf("pos\n");
	}
	return 0;
}
