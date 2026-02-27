#include<stdio.h>
int main()
{
	int x,y,p=1,t;
	scanf("%d%d",&x,&y);t=y;
	while(t--)
	{
		p*=x;
	}
	printf("x:%d power y:%d is :%d\n",x,y,p);
}
