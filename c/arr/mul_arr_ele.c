#include<stdio.h>
void main()
{
int a[5],t[5]={1,1,1,1,1},i,j,ele;
ele=sizeof(a)/sizeof(a[0]);

printf("enter arr eles\n");
for(i=0;i<ele;i++)
scanf("%d",&a[i]);

for(i=0;i<ele;i++)
{
for(j=0;j<ele;j++)
{
if(a[i]==a[j])
continue;
else
t[i]=t[i]*a[j];
}
}

for(i=0;i<ele;i++)
printf("%d ",t[i]);
}
