#include<stdio.h>
#include<stdlib.h>
int main()
{
	FILE *fp=NULL;
	fp=fopen("/dev/basic_char","w");
	char str[]="sai kumar siva";
	fputs(str,fp);
	fclose(fp);
}
