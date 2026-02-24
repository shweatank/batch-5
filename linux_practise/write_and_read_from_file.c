#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc,char ** argv) {
	if(argc != 2) {
		printf("Invalid Argument..!");
		return 0;
	}
	int fd = open(argv[1],O_RDWR|O_CREAT);
	if(fd == -1) {
		printf("Unable to open file..!");
		return 0;
	}
	char buf[] = "This is a test line which is writing into given file";
	char buf2[1024];
	write(fd,buf,strlen(buf)+1);
	lseek(fd,0,SEEK_SET);
	read(fd,buf2,strlen(buf)-20);
	buf2[strlen(buf)-20] = '\0';
	printf("%s\n",buf2);
}

