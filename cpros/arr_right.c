#include<stdio.h>
void main()
{
int i,j,k,n,t;
printf("enter the arr size:\n");
scanf("%d",&n);
int a[n];
for(i=0;i<n;i++)
scanf("%d",&a[i]);

for(i=0,k=n-1;i<=k;i++)
{
if(a[i]==0)
{
t=a[k];
a[k]=a[i];
a[i]=t;
k--;
}
}
for(i=0;i<n;i++)
printf("%d ",a[i]);
printf("\n");
}
