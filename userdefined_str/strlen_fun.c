#include<stdio.h>
#include<string.h>
int strlen_fun(char*s)
{
  static int c=0;
  if(*s=='\0')
  return c;
  else
  {
    c++;
  }  
  strlen_fun(s+1);
}
int main()
{
  char s[]="ramesh reddy";
  int len=strlen(s);
  printf("len=%d\n",len);
  int k=strlen_fun(s);
  printf("k=%d\n",k);
}  
