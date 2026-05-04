#include<stdio.h>
void main()
{
int num=10;
char *p=(char *)&num;
int pos;

for(p=p+3; p>=(char *)&num; p--)
{
for(pos=7;pos>=0; pos--)
printf("%d ",*p>>pos&1);
}
}
