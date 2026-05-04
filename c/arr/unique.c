#include<stdio.h>
void main()
{
int a[5],i,ele;
ele=sizeof(a)/sizeof(a[0]);
printf("enter arr eles\n");
for(i=0;i<ele;i++)
scanf("%d",&a[i]);
int res=0;
for(i=0;i<ele;i++)
{
res=res^a[i];
}
printf("unique ele=%d\n",res);
}
