#include<stdio.h>
#include<stdlib.h>
void main()
{
int *p;
p=calloc(5,sizeof(int));
int i;
for(i=0;i<5;i++)
scanf("%d",&p[i]);

for(i=0;i<5;i++)
printf("%d ",p[i]);
}
