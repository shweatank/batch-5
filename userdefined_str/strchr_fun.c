#include<stdio.h>
#include<stdlib.h>
char* strchr_fun(char*m,char ch)
{
  if(*m=='\0')
  return 0;
  else
 {
   if(*m==ch)
   return m;
 }  
  strchr_fun(m+1,ch);
}
int main()
{
 char m[20]="ramesh reddy";   
 char ch='r';
 char*k=strchr_fun(m,ch);
 printf("%ld\n",k-m);
}
