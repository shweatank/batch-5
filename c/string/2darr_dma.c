#include<stdio.h>
#include<stdlib.h>
void main()
{
int i,j;
int **p=malloc(sizeof(int *)*2);
if(p==0)
{
printf("memory allocation failed\n");
return;
}

for(i=0;i<3;i++)
p[i]=malloc(sizeof(int)*3);

for(i=0;i<2;i++)
for(j=0;j<3;j++)
scanf("%d",&p[i][j]);

for(i=0;i<2;i++)
for(j=0;j<3;j++)
printf("%d",p[i][j]);
}

