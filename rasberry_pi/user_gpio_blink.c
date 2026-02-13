#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE "/dev/led_rw_gpio"

int main() {
    int fd = open(DEVICE, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    int res;
    while(1) {
	read(fd,&res,4); 
	if(res == 1) {
	write(fd, "1", 1);  // Turn LED ON
	sleep(1);
        write(fd, "0", 1);  // Turn LED OFF
        sleep(1);
	}
    }

    close(fd);
    return 0;
}
