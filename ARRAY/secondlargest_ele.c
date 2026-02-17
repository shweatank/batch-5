/*WAP in C to print second largest element in array.
i/p : int a[7]={2,2,3,5,5,4,4};
o/p : second large = 4*/
#include<stdio.h>
int main()
{
  int a[]={2,2,3,5,5,4,4};
  int ele=sizeof(a)/sizeof(a[0]);
  int i=0,j,k,l,sl;
  if(a[0]>a[1])
  {
    l=a[0];
    sl=a[1];
    i=2;
  }
  else if(a[0]<a[1])
 {
   l=a[1];
   sl=a[0];
   i=2;
 }
 else
 {
   l=0;
   sl=0;
   i=0;
 }
 for(  ;i<ele;i++)
 {
   if(a[i]>l)
  {
    sl=l;
    l=a[i];
  }
  else if(a[i]>sl&&l!=sl)
 {
    sl=l;
 }
}
printf("l=%d sl=%d\n",l,sl);
}        
