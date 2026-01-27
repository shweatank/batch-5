#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main()
{
	char str[100];
	puts("enter the string\n");
	scanf("%[^\n]s",str);
	FILE *fp=fopen("/dev/basic_char","w+");
	if(fp==NULL)
	{
		puts("not opened");
		return 0;
	}
	fputs(str,fp);
	int len=strlen(str);
	fseek(fp,-len,SEEK_CUR);
	fgets(str,sizeof(str),fp);
	puts(str);
	//system("$./a.out");
	fclose(fp);


}
