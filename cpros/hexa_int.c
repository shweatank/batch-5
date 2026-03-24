#include"header.h"
int htoi(int num);
typedef int (*fptr)(int num);
fptr p;
int htoi(int num)
{
int value=0,i;
for(i=0;i<32;i++)
{
if(num>>i&1)
{
value|=(1<<i);
}
}

return value;
}
void main()
{
int n,i;
char s[10]="0x123";
if((s[0]=='0'&& s[1]=='X') || (s[0]=='0' && s[1]='x'))
{
for(i=0;s[i];i++)
{


}
}
else
{
printf("string not in hexaformat\n");
return;
}
}

printf("enter number in hexa:\n");
scanf("%x",&n);
p=htoi;
int ret=p(n);
printf("%d\n",ret);
}
