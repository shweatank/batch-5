/*WAP in C to left rotate array 2 times in same array.
i/p: int a[7]={11,21,31,41,51,61,71};
o/p: int a[7]={21,31,41,51,61,71,11}*/
#include<stdio.h>
int main()
{
   int a[7]={11,21,31,41,51,61,71};
   int ele=sizeof(a)/sizeof(a[0]);
   int i,j,k,t;
  for(j=0;j<2;j++)
 { 
   t=a[0];
   for(i=0;i<ele-1;i++)
  {
     a[i]=a[i+1];
  }
  a[i]=t;
 }
  for(i=0;i<ele;i++)
  printf("%d ",a[i]);
}
