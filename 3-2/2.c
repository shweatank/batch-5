#include<stdio.h>
int y;
void main()
{
hi();
}
int hi(void )
{
int x;
if(y==3)
return 0;
else
{
printf("%p\n",&x);
y++;
hi();
}
}
