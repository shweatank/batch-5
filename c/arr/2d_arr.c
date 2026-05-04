#include<stdio.h>
#include<stdlib.h>
void main()
{
int arr[2][3],**p;
int i,j;

p=malloc(sizeof(int *)*2);

for(i=0;i<2;i++)
p[i]=malloc(sizeof(int)*3);

for(i=0;i<2;i++)
{
for(j=0;j<3;j++)
scanf("%d",&p[i][j]);
}

for(i=0;i<2;i++)
{
for(j=0;j<3;j++)
printf("%d",p[i][j]);
}

}
