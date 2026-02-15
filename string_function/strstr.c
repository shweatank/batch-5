#include<stdio.h>
#include<string.h>
char*strstr_fun(char*m,char*s)
{
  int i,j,k;
  for(i=0;m[i];i++)
 {
    for(j=i,k=0;s[k];j++,k++)
   {
      if(m[j]!=s[k])
     {
        break;
     }
   }
   if(s[k]=='\0')
  {
    return (m+i);
  }      
 }
 return 0;
}
int main()
{
  char m[]="ramesh reddy dfhgfd";
  char s[]="reddy";
  char*k=strstr_fun(m,s);
  if(k==0)
 {
   printf("sub string not present\n");
   return 0;
 }
 else
{
  printf("sub string present\n");
  return 0;
}
} 
