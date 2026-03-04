#include<string.h>
#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
	int fd;
	char str[100];
	fd=open("/dev/BASIC_CHAR",O_RDWR);
	if(fd<0)
	{
		perror("open");
		return 0;
	}
	puts("enter the string");
	scanf("%s",str);
	int bytes=write(fd,str,strlen(str)+1);
	printf("bytes write is %d\n",bytes);
	char str2[100];
	bytes=read(fd,str2,sizeof(str2));
	printf("bytes read is %d\n",bytes);
	printf("reading data is %s\n",str2);
	close(fd);
}
