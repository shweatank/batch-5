/*WAP in C to merge 2 array into 3rd array .
i/p: int a[3]={1,5,7}; b[3]={11,22,33};
o/p: int c[6]={1,11,5,22,7,33};*/
#include<stdio.h>
int main()
{
  int a[3]={1,5,7};
  int b[3]={11,22,33};
  int c[6];
  int ele=sizeof(a)/sizeof(a[0]);
  int ele1=sizeof(b)/sizeof(b[0]);
  int i=0,j=0,k=0;
  while(i<ele||j<ele1)
 {
    if(i<ele)
   {
     c[k]=a[i];
     k++;
     i++;
   }
   if(j<ele1)
  {
    c[k]=b[j];
    k++;
    j++;
  }
 }
 for(i=0;i<ele+ele1;i++)
 printf("%d ",c[i]);
}
  
   

