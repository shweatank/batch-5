#include<stdio.h>
#include<string.h>
void strncpy_fun(char*d,char*s,int n)
{
  if(n<=0)
  {
    *d='\0';
     return;
  }
  else
 {
   *d=*s;
 }
  strncpy_fun(d+1,s+1,n-1);
} 
int main()
{
   char m[20]="ramesh reddy";
   char s[20];
   int n=5;
  strncpy_fun(s,m,n);
  printf("%s\n",s);
}  
