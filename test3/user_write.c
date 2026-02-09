#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

struct my_data {
    int id;
    char name[20];
};

int main()
{
    int fd = open("/dev/mydevice", O_WRONLY);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    struct my_data data;
    data.id = 101;
    strcpy(data.name, "Abhi");

    if (write(fd, &data, sizeof(data)) != sizeof(data)) {
        perror("Write failed");
        close(fd);
        return 1;
    }

    printf("Data sent to kernel: id=%d, name=%s\n", data.id, data.name);
    close(fd);
    return 0;
}

