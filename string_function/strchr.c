#include<stdio.h>
char*strchr_fun(char*m,char ch)
{
   int i,j,k;
   while(*m)
  {
    if(*m==ch)
    return m;
    m++;
  }
  return 0;
}
int main()
{
  char m[]="ramesh reddy";
  char ch;
  printf("enter the char\n");
  scanf("%c",&ch);
  char*k=strchr_fun(m,ch);
  if(k==0)
  {
    printf("char not present\n");
  }
  else
 {
  printf("%c %ld\n",*k,k-m);
 }
}
