#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int fd;
	fd = open("/dev/rpi_uart",O_RDWR);
	perror("open: ");
	char ch[] = "Hello";
	char res[6];
	int i = 0;
	while(ch[i]) {
		write(fd,ch+i,1);
		read(fd,res+i,1);
		i++;
	}
	res[i] = '\0';
	printf("Received byte: %s\n",res);
}
