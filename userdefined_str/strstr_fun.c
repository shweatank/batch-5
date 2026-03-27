#include<stdio.h>
#include<string.h>
int strstr_fun(char*m,char*s)
{
  if(*s=='\0')
 {
  return 1;
 }
 else
 {
   if(*m!=*s)
   return 0;
 }
  strstr_fun(m+1,s+1);
}
int strstr_fun1(char*m,char*s)
{
  if(*m=='\0')
 {
   return 0;
 }
 else
 {
   if(*m==*s)
   {
     if(strstr_fun(m,s))
    {
       return 1;
    }
   }
 }
  strstr_fun1(m+1,s);
}
int main()
{
  char m[20]="ramesh reddy ram";
  char s[20]="dd";
  int i,j,k;
  int len=strlen(m);
  int flag;
 /*
 for(i=0;i<len;i++)
 {
   flag=0;
   if(m[i]==s[0])
   {
    k=strstr_fun((m+i),&s[0]);
    if(k)
    {
      flag=1;
      break;
    }
   }
 }               
 if(flag==1)
 { 
	  printf("sub string is present\n");
	  return 0;
  }
  else
  {
	  printf("sub string not present\n");
	  return 0;
  }   */
   
 k=strstr_fun1(m,s);
 if(k)
 { 
	  printf("sub string is present\n");
	  return 0;
  }
  else
  {
	  printf("sub string not present\n");
	  return 0;
  }   
  
}
