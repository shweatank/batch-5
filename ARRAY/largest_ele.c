/* WAP in C to print Smallest element in array.
i/p : int a[6]={2,2,3,5,5,4};
o/p : res = 2*/
#include<stdio.h>
int main()
{
  int a[]={2,2,3,5,5,4,0};
  int ele=sizeof(a)/sizeof(a[0]);
  int i,j,k,l=a[0];
  for(i=1;i<ele;i++)
 {
    if(a[i]>l)
   {
       l=a[i];
   }
 }
 printf("l=%d\n",l);
}   
