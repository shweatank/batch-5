#include<stdio.h>
void main(int argc,char **argv)
{
FILE *sf,*df;
if(argc<3)
{
printf("usage:./a.out srcfile destfile\n");
return;
}
sf=fopen(argv[1],"r");
if(sf==0)
{
printf("no file is present\n");
return;
}

df=fopen(argv[2],"w");
char ch;

while((ch=fgetc(sf))!=EOF)
fputc(ch,df);
}

