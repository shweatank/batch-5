#include<stdio.h>
#include<string.h>

void main(int argc,char **argv)
{
int len;
if(argc<2)
{
printf("usage: ./a.out string\n");
return;
}

len=strlen(argv[1]);
printf("len=%d\n",len);
}
