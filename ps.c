#include<stdio.h>
void main()
{
	int a[]={2,4,3,0,-7};
	int *p=a;
	int *q=&a[3];
	printf("q-p=%d\n",q-p);
	printf("p-q=%d",p-q);
}

