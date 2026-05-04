#include<stdio.h>
void main()
{
int a[5],i,j,ele;
ele=sizeof(a)/sizeof(a[0]);
printf("enter arr ele\n");
for(i=0;i<ele;i++)
scanf("%d",&a[i]);

for(i=0,j=ele-1;i<j;i++)
{
if(a[i]!=a[j])
{
int temp=a[i];
a[i]=a[j];
a[j]=temp;
j--;
}
printf("i=%d\n",i);
printf("j=%d\n",j);
}
printf("after\n");
for(i=0;i<ele;i++)
printf("%d ",a[i]);
}

