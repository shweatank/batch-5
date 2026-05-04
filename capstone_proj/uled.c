#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include<stdlib.h>
#include<string.h>

#define gpio "/dev/led_gpio"
#define spi "/dev/ili9225_char"
int main() {
    int fd;
    char buf[20];
  
    fd = open(gpio, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }
int fd1;
char data[]="Threshold  reached";
fd1=open(spi, O_WRONLY);
if(fd1<0)
{
	perror("failed to open spi file\n");
	return 1;
}
    printf("Waiting for GPIO interrupt...\n");

   
        // Wait for interrupt (blocking)
        read(fd, buf, sizeof(buf));
        printf("Interrupt received! Turning LED ON...\n");

	// playing alsa audio as an indication for alert
        // Turn LED ON
        write(fd, "1", 1);

	// playing alsa audio as an indication for alert
	system("./play_wav test.wav");	
	write(fd1,data,sizeof(data));
        // Keep LED ON for 3 second 
        sleep(3);
//	printf("turning off led\n");

        // Turn LED OFF again
        write(fd, "0", 1);
  
	printf("turning off led\n");

    close(fd);
    close(fd1);
    return 0;
}
