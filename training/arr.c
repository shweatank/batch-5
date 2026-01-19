#include"header.h"
void sort(int*,int);
void (*fptr)(int *,int);
void callback(int*,int,void (*)(int*,int));
void main()
{
int i;
int a[5]={1,10,43,21,2};
callback(a,5,sort);
for(i=0;i<5;i++)
printf("%d ",a[i]);
printf("\n");
}
void callback(int *a,int n,void (*p)(int*,int))
{
p(a,n);
}
void sort(int *a,int n)
{
int i,j,t;
for(i=0;i<n-1;i++)
{
for(j=i+1;j<n;j++)
{
if(a[j]>a[i])
{
t=a[i];
a[i]=a[j];
a[j]=t;
}
}
}
}
