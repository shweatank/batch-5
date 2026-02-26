#include<stdio.h>
#include<stdlib.h>
void main()
{
	char s[]="liki";
	FILE *fd=fopen("/dev/basic_char","w+");
	perror("fopen");
	fwrite(s,sizeof(char),sizeof(s),fd);
	fread(s,sizeof(char),sizeof(s),fd);
	printf("%s\n",s);
	fclose(fd);

}

