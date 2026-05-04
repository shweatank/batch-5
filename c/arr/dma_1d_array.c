#include<stdio.h>
#include<stdlib.h>
void main()
{
int *p=malloc(sizeof(int)*5);
if(p==0)
{
printf("memory allocation failed\n");
return;
}
int i;

printf("enter array eles\n");

for(i=0;i<5;i++)
scanf("%d",&p[i]);


for(i=0;i<5;i++)
printf("%d",p[i]);
}
