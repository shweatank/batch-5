#include"header.h"
void main()
{

char a[]={'a','b','c','d'};
char *p=&a[0];
*p++;
printf("%c %c\n",*++p,--*p);

}
