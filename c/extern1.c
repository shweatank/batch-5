#include<stdio.h>
#include<stdlib.h>
int x=5;
int main()
{
	int *p=&x;
	int x=7;
	printf("x=%d\n",*p);
}
