#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd = open("/dev/gpio_wait", O_RDONLY);

    printf("Waiting for GPIO HIGH (VCC)...\n");
    read(fd, NULL, 0);   // This will BLOCK

    printf("Device Activated! GPIO HIGH detected\n");

    close(fd);
    return 0;
}
