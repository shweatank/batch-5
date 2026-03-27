/*WAP in C to count -ve ,+ve, even , odd ele in array .
i/p: int a[10]={-11,11,12,-12,9,8,-3,10,22};
o/p: +ve = 6 , -ve = 3 , odd = 2 , even = 4*/
#include<stdio.h>
int main()
{
   int a[]={-11,11,12,-12,9,8,-3,10,22};
   int ele=sizeof(a)/sizeof(a[0]);
   int i,j,k;
   int pos=0,odd=0,even=0,neg=0;
   for(i=0;i<ele;i++)
  {
    if(a[i]>0)
   {
      pos++;
     if(a[i]%2==0)
     {
        even++;
     }
     else
     {
        odd++;
     }
   }
   else
  {
    neg++;
   printf("%d ",a[i]);
  }
 }
 printf("pos=%d neg=%d even=%d odd=%d\n",pos,neg,even,odd);
}
   
    
