#include<stdio.h>
int main()
{
  int x=19;
  int y=29;
 printf("Before..\n");
 printf("x=%d y=%d\n",x,y);
 int m,n,i,j;
 for(i=0,j=0;i<=7,j<=7;i++,j++)
{
  m=(x>>i)&1;
  n=(y>>j)&1;
  if(m!=n)
 {
 //  x=x^(1<<i);
   y=y^(1<<j);
 }
}
 printf("After..\n");
 printf("x=%d y=%d\n",x,y);
} 
      
