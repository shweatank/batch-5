#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd = open("/dev/ili9225", O_WRONLY);
    write(fd, "A", 1);
    close(fd);
    return 0;
}

