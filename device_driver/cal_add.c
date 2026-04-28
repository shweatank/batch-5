#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd;
    char write_buf[50];
    char read_buf[50];

    fd = open("/dev/addsub", O_RDWR);
    if (fd < 0) {
        perror("Device open failed");
        return 1;
    }

    printf("Enter two numbers: ");
    scanf("%[^\n]", write_buf);

    write(fd, write_buf, strlen(write_buf));
    read(fd, read_buf, sizeof(read_buf));

    printf("Result from kernel: %s", read_buf);

    close(fd);
    return 0;
}
