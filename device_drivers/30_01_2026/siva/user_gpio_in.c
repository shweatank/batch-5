#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE "/dev/led_gpio1"

int main() {
    char s[2]={0};
    int i=0;
	int fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    while(i<10){
  
	read(fd,s,1);
    	write(fd,s,1);
	sleep(1);
	i++;
    }
    close(fd);
  return 0;
}
