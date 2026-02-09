#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

typedef enum { ADD=0, SUB=1 } operation_t;

struct calc_data {
    int num1;
    int num2;
    operation_t op;
};

int main() {
    int fd;
    struct calc_data data;
    char result[128];
    ssize_t n;

    fd = open("/dev/calc_dev", O_RDWR);
    if (fd < 0) {
        perror("Cannot open device");
        return -1;
    }

    printf("Enter two numbers: ");
    scanf("%d %d", &data.num1, &data.num2);

    int choice;
    printf("Enter operation (0=ADD, 1=SUB): ");
    scanf("%d", &choice);
    data.op = choice;

    write(fd, &data, sizeof(data));

    n = read(fd, result, sizeof(result));
    result[n] = '\0';

    printf("Result from kernel: %s\n", result);

    close(fd);
    return 0;
}

