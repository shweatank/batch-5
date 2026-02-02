#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(int argc,char **argv)
{
	if(argc!=3)
	{
		puts("enter exe filename substr");
		return 0;
	}
	FILE *fp;
	fp=fopen(argv[1],"r+");
	if(fp==NULL)
	{
		puts("file not opened");
		return 0;
	}
	char str[100],*p;
	int len;
	while(fgets(str,100,fp))
	{
		len=strlen(str);
		p=str;
		while(p=strstr(p,argv[2]))
		{
			memset(p,'*',strlen(argv[2]));
		}
		fseek(fp,-len,SEEK_CUR);
		fputs(str,fp);

	}
	fclose(fp);
}
