#include<stdio.h>
void main()
{
int a[5];
int ele=sizeof(a)/sizeof(a[0]);
int i,j,k;
printf("enter arr eles\n");

for(i=0;i <ele; i++)
scanf("%d",&a[i]);

for(i=0;i<ele;i++)
{
for( j=i+1; j<ele ; j++)
{
if(a[i]==a[j])
{
for(k=j; k<ele; k++)
a[k]=a[k+1];
j--;
ele--;
}
}
}

for(i=0; i<ele; i++)
printf("%d ",a[i]);
}
