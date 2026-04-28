#include<stdio.h>


int setbits(int x,int p, int n,int y)
{
	int num=0,i;
	for(i=0;i<n;i++)
	{
		num|=1<<i;
	}
	//x=(x&~(num<<(p-n+1)));
	x=(x&(~(num<<p)));
	num=y&num;
	//x=(x|(num<<(p-n+1)));
	x=(x|(num<<p));
	printf("num is %d\n",x);
	return x;
}	
int main()
{
	setbits(8,6,3,12);
}

