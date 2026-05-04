#include<stdio.h>
#include<stdlib.h>
void main()
{
int ***p=malloc(sizeof(int **)*2);
int i,j,k;

for(i=0;i<2;i++)
{
p[i]=malloc(sizeof(int *)*2);
for(j=0;j<2;j++)
p[i][j]=malloc(sizeof(int)*2);
}

printf("enter arr eles\n");
for(i=0;i<2;i++)
for(j=0;j<2;j++)
for(k=0;k<2;k++)
scanf("%d",&p[i][j][k]);

for(i=0;i<2;i++)
{
for(j=0;j<2;j++)
{
for(k=0;k<2;k++)
{
printf("%d",p[i][j][k]);
}
printf("\n");
}
printf("\n");
}
}


