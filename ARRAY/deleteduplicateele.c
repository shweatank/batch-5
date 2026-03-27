/*WAP in C to delete duplicate elements from array .
i/p: a[10]={3,3,2,4,4,1,2,3,7,9}
o/p: a[10]={3,2,4,1,7,9};*/
#include<stdio.h>
int main()
{
  int  a[]={3,3,2,4,4,1,2,3,7,9};
  int ele=sizeof(a)/sizeof(a[0]);
  int i,j,k;
  for(i=0;i<ele;i++)
 {
   for(j=i+1;j<ele;j++)
  {
     if(a[i]==a[j])
    {
      for(k=j;k<ele;k++)
     {
       a[k]=a[k+1];
     }
     j--;
     ele--;
    }
  }
 }
 for(i=0;i<ele;i++)
 printf("%d ",a[i]);
}       

