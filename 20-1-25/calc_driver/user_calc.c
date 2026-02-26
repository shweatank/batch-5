#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fd;
    char input[100];
    char output[100];

    fd = open("/dev/calc_dev", O_RDWR);

    printf("Enter expression (a + b): ");
    fgets(input, sizeof(input), stdin);

    write(fd, input, sizeof(input));
    read(fd, output, sizeof(output));

    printf("%s", output);

    close(fd);
    return 0;
}

