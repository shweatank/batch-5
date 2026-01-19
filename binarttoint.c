#include<stdio.h>
  #include<string.h>
  int main()
  {
  char bin[200];
  int len,i,res=0;
  printf("enter the string..\n");
  scanf("%s",bin);
  for(len=0;bin[len];len++);
  for(i=0;i<len;i++)
  {
    if(bin[i]!='0' && bin[i]!='1')
    {
     printf("invalid input");
  return 0;
     }
    res=res*2+(bin[i]-'0');
  }
  printf("result:%d",res);
 }

