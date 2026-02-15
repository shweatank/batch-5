#include<stdio.h>
#include<string.h>
void memcpy_fun(void*d,const void*s,size_t n)
{
  char*des=(char*)d;
 const  char*src=(const char*)s;
  int i,j,k;
  for(i=0;i<n;i++)
 {
   des[i]=src[i];
 }
}  
int main()
{
  char s[20]="ramesh reddy";
  char d[20];
  memcpy_fun(d,s,4);
  printf("%s\n",d);
}        
