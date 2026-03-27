#include<stdio.h>
int **function_1()
{
	static int *p;
	return &p;
}
int **function_2()
{
        const int i=10;
	static int *q=&i;
	return &q;
}
void main()
{


	int **p=function_1();
	int **q=function_2();

	printf("%p\n",*p);
	printf("%d\n",**q);
}
