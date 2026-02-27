#include<stdio.h>
int main()
{
	int n,p=1;
	scanf("%d",&n);
	while(p<=n)
	{
		p<<=1;
	}
	p=p>>1;
	printf("largest power <=n is :%d\n",p);
	return 0;
}
