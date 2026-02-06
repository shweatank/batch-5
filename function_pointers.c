#include<stdio.h>
int *fun(int,int);
int sorting(int a[],int ele)
{
  int t,i,j;
  for(i=0;i<ele;i++)
 {
    for(j=i+1;j<ele;j++)
   {
      if(a[i]>a[j])
     {
       t=a[i];
       a[i]=a[j];
       a[j]=t;
     }
   }
 }
 for(i=0;i<ele;i++)
 printf("%d ",a[i]);
}         
int main()
{
  int i,j,k;
  int a[]={3,22,5,1,77,8,44};
  int ele=sizeof(a)/sizeof(a[0]);
  int (*fun)(int*,int)=sorting;
  fun(&a,ele);
}     
       
