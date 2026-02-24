#include<stdio.h>
void sort(int*,int);
typedef void  (*fptr)(int*,int);
void callback(int*,int ,fptr);
fptr p;
void main()
{
int a[5]={1,32,43,22,65};
callback(a,5,sort);
for(int i=0;i<5;i++)
printf("%d ",a[i]);
}
void callback(int *a,int n,fptr p)
{
p(a,n);
}
void sort(int *a,int n)
	{
int t;
for(int i=0;i<n-1;i++)
{
for(int j=0;j<n-1-i;j++)
{
if(a[j]>a[j+1])
{
t=a[j];
a[j]=a[j+1];
a[j+1]=t;
}
}
}
}

