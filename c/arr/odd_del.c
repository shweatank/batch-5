#include<stdio.h>
void main()
{
int a[5],i,j;
int ele=sizeof(a)/sizeof(a[0]);

printf("enter the array eles\n");
for(i=0; i<ele; i++)
scanf("%d",&a[i]);

for(i=0; i<ele; i++)
{
if(a[i]%2!=0)
{
for(j=i; j<ele; j++)
a[j]=a[j+1];
i--;
ele--;
}
}

for(i=0; i<ele; i++)
printf("%d ",a[i]);
}

