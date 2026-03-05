#include "stdio.h"
int main()
{
   char a[] = { 'A', 'B', 'C', 'D' };
   char* ppp = &a[0];
   *ppp++; // Line 1   'b' *(ppp++)
   printf("%c %c ", *++ppp, --*ppp);
printf("%c\n",a[0]); 
}
