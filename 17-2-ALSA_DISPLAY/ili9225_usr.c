#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd = open("/dev/ili9225", O_WRONLY);

    if (fd < 0) {
        perror("open");
        return 1;
    }

    write(fd, "HI", 2);

    close(fd);
    return 0;
}

