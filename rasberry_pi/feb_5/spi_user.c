#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {
	int fd;
	char rdata[6];
	char sdata[] = "hello";
	fd = open("/dev/spi_driver",O_RDWR);
	perror("open");
	int i = 0;
	while(sdata[i]) {
		write(fd,sdata+i,1);
		read(fd,rdata+i,1);
		printf("received data: %c\n",rdata[i]);
		i++;
	}
}
