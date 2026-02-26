#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>

void main()
{
	int fd=open("data2",O_RDWR|O_CREAT|O_APPEND|O_TRUNC,0666);
	if(fd<0)
	{
		perror("open");
		return;
	}

	printf("fd=%d\n",fd);

	int num=100;

	write(fd,&num,sizeof(num));
	perror("write");

	read(fd,&num,sizeof(num));
	perror("read");

	printf("read from file: %d\n",num);
}
