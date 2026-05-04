#include<stdio.h>
#include<stdlib.h>
void main()
{
int **p=malloc(sizeof(int *)*2);
int i,j;

for(i=0;i<2;i++)
p[i]=malloc(sizeof(int)*3);

printf("enter arr eles\n");
for(i=0;i<2;i++)
for(j=0;j<3;j++)
scanf("%d",&p[i][j]);

for(i=0;i<2;i++)
for(j=0;j<3;j++)
printf("%d ",p[i][j]);


}

