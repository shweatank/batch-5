#include<stdio.h>
void main()
{
int a[5],i,j,ele;
ele=sizeof(a)/sizeof(a[0]);

printf("enter the eles\n");
scanf("%d",&a[i]);

for(i=0,j=ele-1;i<j; )
{
if(a[i]!=a[j])
{
int temp=a[i];
a[i]=a[j];
a[j]=temp;
i++;
j--;
}
}
for(i=0;i<ele;i++)
printf("%d ",a[i]);
}

