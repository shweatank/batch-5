#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main() {
	int fd;
	fd = open("./files.txt",O_RDWR);
	perror("open\n");
	char str[100] = "dummy text to be written in file using write call";
	write(fd,str,strlen(str)+1);
	perror("write\n");

	memset(str,0,100);
	lseek(fd,0,SEEK_SET);
	read(fd,str,50);
	perror("read\n");
	printf("%s\n",str);
}


