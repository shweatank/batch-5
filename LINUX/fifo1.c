#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main()
{
	char str[10];
	if((mkfifo("hfd",0664))==-1)
	{
		perror("mkfifo");
	}
	int fdw;
	fdw=open("hfd",O_WRONLY);
	while(1)
	{
	fgets(str,10,stdin);if((str[strlen(str)-1])==10){str[strlen(str)-1]=0;}
	if((write(fdw,str,strlen(str)+1))==0)
	{
		printf("writer not written\n");
		return 0;
	}
	puts("p1 written");
	}
}
