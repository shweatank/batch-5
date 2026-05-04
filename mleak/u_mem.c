#include<stdio.h>
#include<stdlib.h>
void main()
{
int *p=malloc(sizeof(int));
printf("enter the num\n");
scanf("%d",p);
printf("num=%d\n",*p);
//free(p);
}
