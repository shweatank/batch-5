#include<stdio.h>
void main()
{
	int a[]={3,3,6,7,0,5};
	int *p;
	p=a;
	printf("%d\n%d\n%d\n%d\n",*p++,*++p,*p--,*--p);
	printf("%d\n",*p);
}

