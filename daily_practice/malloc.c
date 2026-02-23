#include<stdlib.h>
#include<stdio.h>
int main()
{
	int *a=malloc(100);
	int *p=a;
	printf("%p\n",a);
	free(a);
	int *q=malloc(100);
	p=realloc(p,100);
	printf("%p\n",p);

}
