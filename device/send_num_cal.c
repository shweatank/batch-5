#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main(int argc,char **argv)
{
	if(argc!=2)
	{
		puts("enter the exe and operation");
		return 0;
	}
	FILE *fp=fopen("/dev/basic_char","w+");
	if(fp==NULL)
	{
		puts("file is not opened");
		return 0;
	}
	char buff[100];
	strcpy(buff,argv[1]);
	int len=strlen(buff);
	fputs(buff,fp);
	fseek(fp,-len,SEEK_CUR);
	memset(buff,0,strlen(buff)+1);
	fgets(buff,sizeof(buff),fp);
	int num=atoi(buff);
	printf("caluculated is %d\n",num);
	fclose(fp);

}
