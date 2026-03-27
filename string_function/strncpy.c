#include<stdio.h>
void strncpy_fun(char*d,char*s,int n)
{
  int i,j,k;
  while(*s&&n>0)
 {
    *d=*s;
     d++;
     s++;
     n--;
 }
 *d='\0';
}
int main()
{
  char m[20]="ramesh reddy";
  char d[20];
  printf("%s\n",m);
  int n=5;
  strncpy_fun(d,m,n);
  printf("%s\n",d);
} 
