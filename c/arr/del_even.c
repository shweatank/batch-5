#include<stdio.h>
void main()
{
int a[5],i,j,ele;
ele=sizeof(a)/sizeof(a[0]);
printf("enter the ele\n");
for(i=0;i<ele;i++)
scanf("%d",&a[i]);

for(i=0;i<ele;i++)
{
if(a[i]%2==0)
{
for(int j=i;j<ele;j++)
a[j]=a[j+1];
ele--;
i--;
}
}


for(i=0;i<ele;i++)
printf("%d ",a[i]);
}

