/*WAP in C to print second smallest element in array.
i/p : int a[7]={2,2,3,5,5,4,4};
o/p : second large = 4*/
#include<stdio.h>
int main()
{
  int a[]={2,2,1,5,5,3,3};
  int ele=sizeof(a)/sizeof(a[0]);
  int i=0,j,k,s=a[0],ss;
  for(i=1;i<ele;i++)
 {
   if(a[i]<s)
   {
      ss=s;
      s=a[i];
   }
 }
 printf("ss=%d\n",ss);
}
  
