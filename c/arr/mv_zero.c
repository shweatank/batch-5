#include<stdio.h>
void main()
{
int a[5],i,j,ele;
ele=sizeof(a)/sizeof(a[0]);
printf("enter arr eles\n");
for(i=0;i<ele;i++)
scanf("%d",&a[i]);
i=0;
for(j=0;j<ele;j++)
{
if(a[j]==0)
{
int t=a[i];
a[i]=a[j];
a[j]=t;
i++;
}
}

for(i=0;i<ele;i++)
printf("%d ",a[i]);
printf("\n");
}
