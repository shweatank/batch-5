#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>

int main() {
    int fd = open("/dev/spi_data", O_RDWR);
    if(fd < 0) { perror("open"); return 1; }

    char buf[10]="gurunath";
    write(fd, buf, strlen(buf));   // write 'Z'

    read(fd, buf, 10);    // read back
    printf(" %s\n", buf);

    close(fd);
    return 0;
}

