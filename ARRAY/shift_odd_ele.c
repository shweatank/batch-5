/*43 .WAP in C to shift all odd elements to right side
in same array.
i/p: int a[7]={11,22,22,44,55,33,88};
o/p: int a[7]={22,22,44,88,11,55,33}*/
#include<stdio.h>
int main()
{
   int a[7]={11,22,22,44,55,33,88};
   int ele=sizeof(a)/sizeof(a[0]);
   int i,j,k,t;
   for(i=0;i<ele;i++)
   printf("%d ",a[i]);
   printf("\n");
   k=0;
   for(i=0;i<ele-k;i++)
  {
    if(a[i]%2!=0)
   {
      k++;
      t=a[i];
      for(j=i;j<ele;j++)
     {
       a[j]=a[j+1];
     }
     a[ele-1]=t;
     i--;
   }
  }
   for(i=0;i<ele;i++)
   printf("%d ",a[i]);
   printf("\n");
}

