#include<stdio.h>
int (*fptr)(int,int);

int sum(int x,int y)
{
	return x+y;	
}

void* calculate(int x,int y)
{
	fptr=sum(x,y);
	return fptr;
}

int main()
{
	int x=10,y=20;
	printf("sum : %d\n",calculate(x,y));
}
