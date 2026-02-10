#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE "/dev/led_gpio"

int main() {
	int fd = open(DEVICE, O_WRONLY|O_RDONLY);
	if (fd < 0) {
		perror("Failed to open device");
		return 1;
	}
	int status;
	for (int i = 0; i < 100; i++) {
		read(fd,&status,sizeof(int));
		perror("read");
		if(status)
			write(fd,"1",1);
		else
			write(fd,"0",1);
		sleep(2);
	}

    /*
int i;
    while(1){
	    read(fd,&i,sizeof(i));
    }
    */
    close(fd);
    return 0;
}
