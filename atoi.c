#include<stdio.h>
#include<string.h>
int main()
{
  char s[20];
  printf("enter the string\n");
  scanf("%s",s);
  int i=0,j,k,num=0;
  int len=strlen(s);
  if(s[0]=='+'||s[0]=='-')
 {
    i=1;
 }
 
  for( ;i<len;i++)
  {
     num=num*10+(s[i]-48);
  }

if(s[0]=='-')
num=-num;
printf("%d\n",num);
}    
