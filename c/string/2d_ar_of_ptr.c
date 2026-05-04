#include<stdio.h>
#include<stdlib.h>
void main()
{
int *p[2];
int i,j;

for(i=0;i<2;i++)
p[i]=malloc(sizeof(int *));

for(j=0;j<2;j++)
p[j]=malloc(sizeof(int)*2);

for(i=0;i<2;i++)
for(j=0;j<2;j++)
scanf("%d",&p[i][j]);

for(i=0;i<2;i++)
for(j=0;j<2;j++)
printf("%d",p[i][j]);
}

