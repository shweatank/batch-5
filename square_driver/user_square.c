#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(void) {
    int fd;
    int num;
    ssize_t w, r;
    char result[64];

    fd = open("/dev/square_dev", O_RDWR);
    if (fd < 0) {
        perror("Cannot open /dev/square_dev");
        return 1;
    }

    printf("Enter an integer: ");
    if (scanf("%d", &num) != 1) {
        fprintf(stderr, "Invalid input\n");
        close(fd);
        return 1;
    }

   
    w = write(fd, &num, sizeof(num));
    if (w != sizeof(num)) {
        if (w < 0) perror("write failed");
        else fprintf(stderr, "Unexpected write size: %zd\n", w);
        close(fd);
        return 1;
    }

  
    r = read(fd, result, sizeof(result) - 1);
    if (r < 0) {
        perror("read failed");
        close(fd);
        return 1;
    }

    result[r] = '\0';
    printf("Square from kernel: %s\n", result);

    close(fd);
    return 0;
}

