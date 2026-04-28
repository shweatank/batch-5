#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("invalid argument\n");
		return 0;
	}
	if(mkfifo(argv[1],0666)==-1)
	{
		if(errno!=EEXIST)
                {
                        perror("mkfifo");
                        return 0;
                }

	}
	printf("fifo is opend for communication\n");
	int fd=open(argv[1],O_WRONLY);
	if(fd==-1)
	{
		if(errno!=EEXIST)
                {
                        perror("mkfifo");
                        return 0;
                }
	}
	printf("file is opened for writing\n");
	char a[10],len;
	while(1)
	{
		scanf("%s",a);
		len=strlen(a)+1;
		if(write(fd,a,len)!=len)
		{
			perror("write");
		}
		if(strcmp(a,"quit")==0)
		{
			break;
		}
	}
	close(fd);
	return 0;
}
