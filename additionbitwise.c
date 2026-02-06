#include<stdio.h>
int main()
{
  int a=12,b=10,c;
  while(b!=0)
 {
   c=a&b;
   a=a^b;
   b=c<<1;
 }
 printf("%d",a);
}      
              
