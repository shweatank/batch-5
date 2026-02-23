#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>

int main() {
    int fd = open("/dev/mouse_touch", O_RDONLY);
    if(fd < 0) { perror("open"); return 1; }

    char buf[64];
    while(1){
        int n = read(fd, buf, sizeof(buf)-1);
        if(n > 0){
            buf[n] = 0;
            // send buf to ILI9225 via /dev/spidev0.1
            printf("String to display: %s\n", buf);
        }
        sleep(1);
    }
    close(fd);
}
