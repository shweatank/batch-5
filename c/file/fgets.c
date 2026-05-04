#include<stdio.h>
void main()
{
char s[20];
FILE *fp=fopen("src","r");
if(fp==0)
{
printf("no file is present\n");
return;
}

fgets(s,10,fp);

printf("file content=%s\n",s);
}

