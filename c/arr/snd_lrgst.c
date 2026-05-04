#include<stdio.h>
void main()
{
int a[5],i,l,sl,ele;
ele=sizeof(a)/sizeof(a[0]);

printf("enter arr eles\n");
for(i=0;i<ele;i++)
scanf("%d",&a[i]);

l=a[0];
for(i=1;i<ele;i++)
{
if(a[i]>l)
{
sl=l;
l=a[i];
}
}

printf("lar=%d sl=%d\n",l,sl);
}
