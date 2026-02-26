#include<stdio.h>
#include<string.h>
void main()
{
	char s[]="liki";
	FILE *fp=fopen( " ","w");
        fwrite(s,sizeof(char),sizeof(s),fp);
	fread(s,sizeof(char),sizeof(s),fp);
	printf("%s",s);
}

