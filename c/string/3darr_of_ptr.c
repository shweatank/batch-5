#include<stdio.h>
#include<stdlib.h>
void main()
{
int *p[2][2];
int i,j,k;
for(i=0;i<2;i++)
for(j=0;j<2;j++)
p[i][j]=malloc(sizeof(int)*3);

for(i=0;i<2;i++)
for(j=0;j<2;j++)
for(k=0;k<3;k++)
scanf("%d",&p[i][j][k]);

for(i=0;i<2;i++)
for(j=0;j<2;j++)
for(k=0;k<3;k++)
printf("%d",p[i][j][k]);

}
