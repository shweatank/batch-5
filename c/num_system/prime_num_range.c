#include<stdio.h>
void main()
{
int s,e;
printf("enter the s raange\n");
scanf("%d",&s);
printf("enter the end range\n");
scanf("%d",&e);
int i,j;
for(i=s;i<e;i++)
{
for(j=2;j<e;j++)
{
if(i%j==0)
break;
}
if(j==i)
printf("%d ",i);
//printf("\n");
}
printf("\n");
}
