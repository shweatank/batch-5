/*WAP in C to delete -ve elements from array .
i/p: int a[6]={-11,12,-14,13,-15,-18};
o/p: int a[6]={12, 13};*/
#include<stdio.h>
int main()
{
   int a[]={-11,-12,-14,13,-15,-18};
   int ele=6;
   int i,j,k;
   for(i=0;i<ele;i++)
  {
    if(a[i]<0)
   {
     for(j=i;j<ele;j++)
    {
      a[j]=a[j+1];
    }
    i--;
    ele--;
   }
  }
//  printf("ele=%d\n",ele);
  for(i=0;i<ele;i++)
  printf("%d ",a[i]);
}  

