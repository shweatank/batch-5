#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define CALC_IOCTL _IOWR('k', 1, struct calc_data)

struct calc_data {
    int a;
    int b;
    char op;
    int result;
};

int main()
{
    int fd;
    struct calc_data data;

    fd = open("/dev/basic_cal_ker", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    data.a = 10;
    data.b = 5;
    data.op = '*';

    ioctl(fd, CALC_IOCTL, &data);

    printf("Result = %d\n", data.result);

    close(fd);
    return 0;
}
