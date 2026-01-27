#include"header.h"
void main()
{
	int r=mkfifo("f1",0664);
	perror("mkfifo");
	int fd=open("f1",O_WRONLY);
	char s[10];
	printf("enter the data..\n");
	scanf("%s",s);
	write(fd,s,strlen(s)+1);
}
