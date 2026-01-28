#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"
int main()
{
    int fd;
    struct data data;
    int result;

    fd = open("/dev/kb_intr1", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    printf("Enter first number: ");
    scanf("%d", &data.a);

    printf("Enter second number: ");
    scanf("%d", &data.b);

    printf("\nNow press one key on keyboard:\n");
    printf("A → Addition\n");
    printf("B → Subtraction\n");
    printf("M → Multiplication\n");
    printf("D → Division\n");
    printf("Then press ENTER in terminal...\n");

    /* Send operands to kernel */
    if (write(fd, &data, sizeof(data)) < 0) {
        perror("write");
        close(fd);
        return 1;
    }

    /* Read result from kernel */
    if (read(fd, &result, sizeof(result)) < 0) {
        perror("read");
        close(fd);
        return 1;
    }

    printf("\nResult from kernel = %d\n", result);

    close(fd);
    return 0;
}

