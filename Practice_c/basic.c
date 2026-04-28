#include<stdio.h>

static int a;
int main()
{
	static int a=10;
	printf("%d\n",a);
}
