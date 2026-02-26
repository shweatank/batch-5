#include<stdio.h>
#include<stdlib.h>
void main()
{
	int *p;
	p=malloc(sizeof(int));
	printf("enter the val\n");
	scanf("%d",p);
	printf("\n");
	printf("%d\n",*p);
}
