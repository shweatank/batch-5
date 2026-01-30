#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
int main()
{
	FILE *fp;
	fp=fopen("sai.txt","r");
	if(fp==NULL)
	{
		puts("not opened");
		return 0;
	}
	char str[200];
	long size;
	int f=0;
	while(fscanf(fp,"%s",str)!=EOF)
	{
		printf("%s\n",str);
	}
	size=ftell(fp);
	while(1)
	{
		fseek(fp,size,SEEK_SET);
		if(fgets(str,200,fp))
		{
			do
			{
		printf("%s",str);
		fflush(stdout);
			}while(fgets(str,200,fp));
		}
		else
		{
			sleep(1);
		}
		size=ftell(fp);
	}
}
