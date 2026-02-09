#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    char buf[100];
    
    fd = open("/dev/reverse_dev", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    printf("Enter string: ");
    fgets(buf, sizeof(buf), stdin);

    // remove newline
    buf[strcspn(buf, "\n")] = '\0';

    write(fd, buf, strlen(buf));

    int n = read(fd, buf, sizeof(buf)-1);
    buf[n] = '\0'; // null-terminate

    printf("Reverse string: %s\n", buf);

    close(fd);
    return 0;
}

