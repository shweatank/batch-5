#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    char buf[512];

    fd = open("/dev/kmsg", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    /* Seek to end so we only see new messages */
    lseek(fd, 0, SEEK_END);

    while (1) {
        int n = read(fd, buf, sizeof(buf) - 1);
        if (n > 0) {
            buf[n] = '\0';
            printf("%s", buf);
        }
    }

    close(fd);
    return 0;
}

