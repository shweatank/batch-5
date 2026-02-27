#include<stdio.h>
int main()
{
	int n,c=0;
	scanf("%d",&n);
	if(n&&((n&(n-1))==0)&&(n&0x55555555))
	{
		printf("pow of 4\n");
	}
	else
		printf("not a power of 4\n");
	return 0;
}
