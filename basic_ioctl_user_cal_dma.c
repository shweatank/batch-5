// basic_ioctl_user.c
// Minimal user program calling ioctl

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

struct st{
	int a;
	int b;
	int res;
}v;
#define IOCTL_MAGIC 'B'
//#define IOCTL_SET_VALUE _IOWR(IOCTL_MAGIC, 1, int);

#define ADD _IOWR(IOCTL_MAGIC, 1, struct st)
#define SUB _IOWR(IOCTL_MAGIC, 2,struct st)
#define MUL _IOWR(IOCTL_MAGIC, 3, struct st)
#define DIV _IOWR(IOCTL_MAGIC, 4, struct st)
int main(void)
{
    v.a=10;v.b=20;
    int fd;
    int value = 20;

    fd = open("/dev/basic_ioctl", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    printf("User: sending structure to kernel\n");

    ioctl(fd,ADD, &v);

    printf("User: got back result %d from kernel\n", v.res);

    close(fd);
    return 0;
}
