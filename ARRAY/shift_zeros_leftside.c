/*44.WAP in C to shift all 0 to left side in same array.
i/p: int a[7]={11,0,0,44,0,33,0};
o/p: int a[7]={0,0,0,0,11,44,33};*/
#include<stdio.h>
int main()
{
   int a[7]={11,0,0,44,0,33,0};
   int ele=sizeof(a)/sizeof(a[0]);
   int i,j,k,t;
   for(i=0;i<ele;i++)
   printf("%d ",a[i]);
   printf("\n");
   k=0;
   for(i=0;i<ele-k;i++)
  {
    if(a[i]!=0)
   {
     k++;
     t=a[i];
    for(j=i;j<ele;j++)
    {
       a[j]=a[j+1];
    }
    i--;
    a[ele-1]=t;
   }
  }
   for(i=0;i<ele;i++)
   printf("%d ",a[i]);
   printf("\n");
}
                  
    
