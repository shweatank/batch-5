#include<stdio.h>
void main()
{
int a[10],i,j,ele;
ele=sizeof(a)/sizeof(a[0]);
printf("enter the el\n");
for(i=0;i<ele;i++)
scanf("%d",&a[i]);

for(i=0;i<ele;i++)
{
for(j=i+1;j<ele;j++)
{
if(a[i]>a[j])
{
int temp=a[i];
a[i]=a[j];
a[j]=temp;
}
}
}

for(i=0;i<ele;i++)
printf("%d ",a[i]);
printf("\n");
}
