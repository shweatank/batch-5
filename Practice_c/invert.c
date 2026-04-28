#include<stdio.h>

int invert(int x,int p,int n)
{
	int num=0,i;
	for(int i=p;i<n;i++)
	{
		x^=~(1<<i);
	}
	printf("%d\n",x);

}
int main()
{
	invert(8,0,4);
}
