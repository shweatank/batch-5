#include<stdio.h>
#include<fcntl.h>
#include<stdio_ext.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
int main()
{
	int fd;
	fd=open("sai.txt",O_CREAT|O_WRONLY|O_APPEND,0666);
	if(fd<0)
	{
		perror("open");
		return 0;
	}
	char str[100];
	for(int i=0;i<2;i++)
	{
		printf("enter the string write into file\n");
		scanf("%[^\n]s",str);
		__fpurge(stdin);
		write(fd,str,strlen(str));
		write(fd,"\n",1);
	}
	close(fd);
}
