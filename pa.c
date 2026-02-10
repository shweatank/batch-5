#include<stdio.h>
void main()
{
	int a[5]={1,4,5,8,0};
	int *p=&a[0];
	printf("value is :%d",*p);
	printf("address of element is :%u\n",p);
	p=p+2;
	printf("value is :%d",*p);
	printf("address of elementis :u\n",p);
}

