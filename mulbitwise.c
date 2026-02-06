#include<stdio.h>
int main()
{
  int a=3,b=4,res=0;
  while(b!=0)
 {
    if(b&1==1)
   {
     res=res+a;
   }
   a=a<<1;
   b=b>>1;
 }
 printf("%d",res);
} 
      
                  
