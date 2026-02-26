#include<stdio.h>
void main()
{
int a[]={7,3,8,2,1,9},ele,i,L,SL;
ele=sizeof(a)/sizeof(a[0]);
if(a[0]>a[1])
{
L=a[0];
SL=a[1];
}
else if(a[1]>a[0])
{
L=a[1];
SL=a[0];
}
for(i=2;i<ele;i++)
{
if(a[i]>L)
{
SL=L;
L=a[i];
}
else if(a[i]>SL &&a[i]!=L)
SL=a[i];
}
printf("Largest=%d and sec largest=%d\n",L,SL);
}
