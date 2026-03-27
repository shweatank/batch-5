#include<stdio.h>
void strcpy_fun(char*d,char*s)
{
  int i,j,k;
  while(*s)
 {
    *d=*s;
     d++;
     s++;
 }
 *d='\0';
}
int main()
{
  char m[20]="ramesh reddy";
  char d[20];
  printf("%s\n",m);
  strcpy_fun(d,m);
  printf("%s\n",d);
} 
