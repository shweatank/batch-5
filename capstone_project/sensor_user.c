#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    char buf[64];

    fd = open("/dev/dht11", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    read(fd, buf, sizeof(buf));
    printf("%s", buf);

    close(fd);
    return 0;
}

