#include<stdio.h>
#include<stdlib.h>
void main()
{
int *p;
p=malloc(sizeof(int)*5);
int i;
for(i=0;i<5;i++)
scanf("%d",(p+i));

for(i=0;i<5;i++)
printf("%d",*(p+i));
}
