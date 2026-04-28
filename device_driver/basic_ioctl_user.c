// basic_ioctl_user.c
// Minimal user program calling ioctl

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define IOCTL_MAGIC 'B'
#define IOCTL_SET_VALUE _IOWR(IOCTL_MAGIC, 1, char)

int main(void)
{
    int fd;
    char value[20];

    fd = open("/dev/str_ioctl", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }
printf("enter the string:");
scanf("%[^\n]s",value);
    printf("User: sending %s to kernel\n", value);

    ioctl(fd, IOCTL_SET_VALUE, value);

    printf("User: got back %s from kernel\n", value);

    close(fd);
    return 0;
}
