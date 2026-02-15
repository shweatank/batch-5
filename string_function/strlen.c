#include<stdio.h>
int strlen_fun(char*m)
{
  int c=0;
  while(*m)
 {
   c++;
   m++;
 }
 return c;
}
int main()
{
 char s[]="ramesh reddy";
 int k=strlen_fun(s);
 printf("k=%d\n",k);
} 
