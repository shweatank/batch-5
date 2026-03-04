#include<stdio.h>

void Find_set(int x,int y)
{
	int cnt=0;
	x=x^y;
	while(x)
	{
		x=x&(x-1);
		cnt++;
	}
	printf("%d\n",cnt);
}

int main()
{
	int x,y;
	printf("enter the x value\n");
	scanf("%d",&x);
	printf("enter the y value\n");
	scanf("%d",&y);
	Find_set(x,y);
}
