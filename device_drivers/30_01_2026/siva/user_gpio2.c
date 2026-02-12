#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE "/dev/led_gpio1"

int main() {
	char b[2];
    int fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    for (int i = 0; i < 10; i++) {
	read(fd,b,1);
	    write(fd, b, 1);  // Turn LED ON
        sleep(1);
    }
    close(fd);
    return 0;
}
