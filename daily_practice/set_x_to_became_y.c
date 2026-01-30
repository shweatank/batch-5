#include<stdio.h>
int main()
{
	int x,y;
	puts("enter the x value");
	scanf("%d",&x);
	puts("enter y to x became y");
	scanf("%d",&y);
	int i,cnt=0;
	for(i=31;i>=0;i--)
	{
		if(((y>>i)&1)==1 && ((x>>i)&1)==0)
		{
			x|=1<<i;
			cnt++;
			continue;
		}
		if(((y>>i)&1)==0 && ((x>>i)&1)==1)
		{
			x=x&~(1<<i);
			cnt++;
		}
	}
	printf("x=%d y=%d %d\n",x,y,cnt);
}
