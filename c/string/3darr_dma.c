#include<stdio.h>
#include<stdlib.h>
void main()
{
int i,j,k;
int ***p=malloc(sizeof(int **)*2);
if(p==0)
{
printf("failed\n");
return;
}

for(i=0;i<2;i++)
p[i]=malloc(sizeof(int *)*2);

for(i=0;i<2;i++)
for(j=0;j<2;j++)
p[i][j]=malloc(sizeof(int)*2);

for(i=0;i<2;i++)
for(j=0;j<2;j++)
for(k=0;k<2;k++)
scanf("%d",&p[i][j][k]);

for(i=0;i<2;i++)
for(j=0;j<2;j++)
for(k=0;k<2;k++)
printf("%d",p[i][j][k]);
}
