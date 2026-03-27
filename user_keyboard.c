#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("/dev/kbd9dev", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    char buf[16];
    while (1) {
        int n = read(fd, buf, sizeof(buf));
        if (n > 0) {
            buf[n] = '\0';
            printf("Key pressed: %s", buf);
        }
    }

    close(fd);
    return 0;
}

