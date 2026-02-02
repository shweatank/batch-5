#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void Rev(char *,char *);
int main(int argc,char **argv)
{
	if(argc!=3)
	{
		puts("enter exe filename substring");
		return 0;
	}
	FILE *fp=fopen(argv[1],"r+");
	if(fp==NULL)
	{
		puts("file is not opened");
		return 0;
	}
	char str[100],rev[50],*p;
	Rev(argv[2],rev);
	int len=strlen(argv[2]),len2;
	while(fgets(str,100,fp))
	{
		len2=strlen(str);
		p=str;
		while(p=strstr(p,argv[2]))
		{
			memcpy(p,rev,len);
			p=p+len;
		}
		fseek(fp,-len2,SEEK_CUR);
		fputs(str,fp);
	}
	fclose(fp);
}
void Rev(char *src,char *dest)
{
	int i=0,j=strlen(src)-1;
	for(;i<strlen(src);i++,j--)
	{
		dest[i]=src[j];
	}
	dest[i]='\0';
	printf("%s %s",dest,src);
}

