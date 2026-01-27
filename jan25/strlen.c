#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>

int main()
{
	int fd=open("/dev/basic_char",O_RDWR);
	if(fd<0)
	{
		perror("open");
		exit(0);
	}

	char str[100];
	puts("enter string : ");
	scanf("%s",str);

	write(fd,str,sizeof(str));
	char ptr[100];
	read(fd,ptr,sizeof(ptr));
	printf("string : %s\n",ptr);
}
