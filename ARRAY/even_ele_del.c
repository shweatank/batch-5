/*WAP in C to delete even duplicate ele from array.
i/p: a[10]={3,3,2,4,4,2,5,3,4,9}
o/p: a[10]={3,3,2,4,5,3,9};*/
#include<stdio.h>
int main()
{
  int  a[10]={3,3,2,4,4,2,5,3,4,9};
  int ele=sizeof(a)/sizeof(a[0]);
  int i,j,k;
  for(i=0;i<ele;i++)
 {
   if(a[i]%2==0)
   {
     for(k=i;k<ele;k++)
     {
       a[k]=a[k+1];
     }
     i--;
     ele--;
   }
  }
 for(i=0;i<ele;i++)
 printf("%d ",a[i]);
}       

    
   
