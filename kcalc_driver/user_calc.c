#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd;
    char input[100];
    char output[100];
    int n;

    fd = open("/dev/kcalc", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    printf("Enter operation (example: 10 5 +): ");
    fgets(input, sizeof(input), stdin);

    /* Send user input to kernel */
    write(fd, input, strlen(input));

    /* Read result from kernel */
    n = read(fd, output, sizeof(output) - 1);
    if (n > 0) {
        output[n] = '\0';   // null terminate
        printf("Kernel says: %s", output);
    }

    close(fd);
    return 0;
}

