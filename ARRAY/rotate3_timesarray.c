/*WAP in C to right rotate array 3 times in same array.
i/p: int a[7]={11,21,31,41,51,61,71};
o/p: int a[7]={51,61,71,11,21,31,41}*/
#include<stdio.h>
int main()
{
  int a[7]={11,21,31,41,51,61,71};
  int ele=sizeof(a)/sizeof(a[0]);
  int i,j,k,t;
  for(j=0;j<3;j++)
 {
   t=a[ele-1];
  for(i=ele-1;i>0;i--)
 {
   a[i]=a[i-1];
 }
 a[i]=t;
}
for(i=0;i<ele;i++)
printf("%d ",a[i]);
}     
     
