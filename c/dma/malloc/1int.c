#include<stdio.h>
#include<stdlib.h>

void main()
{

int *p;
p=malloc(sizeof(int));
printf("enter the integer\n");
scanf("%d",p);

printf("num=%d\n",*p);
}

