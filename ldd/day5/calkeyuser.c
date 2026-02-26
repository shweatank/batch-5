#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("/dev/kbd_calc", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    char buf[128];
    int n = read(fd, buf, sizeof(buf) - 1);
    if (n > 0) {
        buf[n] = 0;
        printf("%s", buf);
    }

    close(fd);
    return 0;
}


