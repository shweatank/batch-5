#include<stdio.h>
void main()
{
int a;
char ch;
char s[]="10a";
sscanf(s,"%d %c",&a,&ch);

printf("%d %c\n",a,ch);
}
