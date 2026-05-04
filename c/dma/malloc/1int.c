#include<stdio.h>
#include<stdlib.h>
void main()
{
int *p;
p=malloc(sizeof(int));

scanf("%d",p);
printf("%d\n",*p);
}
