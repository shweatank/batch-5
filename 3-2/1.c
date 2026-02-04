#include<stdio.h>
void main()
{
char c[2];

if(&c[1]-&c[0]<0)
printf("down\n");
else
printf("up\n");
int x=0xffff;
x&=~(0xffffffff);
printf("%d\n",x);

}
