#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE "/dev/led_gpio"

int main() {
    int fd = open(DEVICE, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }
int status;
while(1)
{
read(fd,&status,sizeof(int));
if(status)
	write(fd,"1",1);
else
	write(fd,"0",0);

}
    close(fd);
    return 0;
}
