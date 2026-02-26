#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd;
    char write_buf[100];
    char read_buf[100];

    fd = open("/dev/reverse_dev", O_RDWR);

    printf("Enter string: ");
    fgets(write_buf, sizeof(write_buf), stdin);

    write(fd, write_buf, strlen(write_buf));
    read(fd, read_buf, sizeof(read_buf));

    printf("Reversed string: %s\n", read_buf);

    close(fd);
    return 0;
}

