#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

struct st {
	int status;
};

struct st var={0},var2;


int main()
{

	int fd=open("/dev/key_board_file",O_RDWR);
	perror("open");

	write(fd,&var,sizeof(var));
	perror("write");

	read(fd,&var2,sizeof(var2));
	perror("read");

	printf("%d",var2.status);
}

        

