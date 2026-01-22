#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
int main()
{
	FILE *fp=fopen("sai.txt","r");
	if(fp==NULL)
	{
		perror("fopen");
		return 0;
	}
	char str[100];
	fread(str,100,1,fp);
	fclose(fp);
	fp=fopen("sai.txt","w");
	fwrite(str,strlen(str)+1,1,stdout);
	fclose(fp);
}
