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

    char rs[2];
    int i = 0;
    while(1) {
	read(fd,rs,1); 
	if(i <= 1024) printf("%c  ",rs[0]);
	if(rs[0] =='1'){
	write(fd, "1", 1);  // Turn LED ON
	sleep(1);
        write(fd, "0", 1);  // Turn LED OFF
        sleep(1);
	}
	i = i << 1;
    }

    close(fd);
    return 0;
}
