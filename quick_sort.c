#include<stdio.h>
int main()
{
   int i,j,k,t,k1;
  int a[]={5,10,9,2,3,5,15,16,1,6};
  int ele=sizeof(a)/sizeof(a[0]);
  for(i=0;i<ele;i++)
  printf("%d ",a[i]);
  printf("\n");
  i=0;
  int pivot=a[0];
  j=ele-1;
 while(i<=j)
{
  while(a[i]<=pivot)
 {
   i++;
 }
 while(a[j]>pivot)
{
  j--;
}
if(i<j)
{
 t=a[i];
 a[i]=a[j];
 a[j]=t;
}
}
if(i>j)
{
  t=a[0];
  a[0]=a[j];
  a[j]=t;
} for(i=0;i<ele;i++)
 printf("%d ",a[i]);
}         
