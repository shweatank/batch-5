#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define GPIO_PIN "17"

void write_sysfs(const char *path, const char *value)
{
    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        perror(path);
        exit(1);
    }
    write(fd, value, strlen(value));
    close(fd);
}

int main(void)
{
    /* Export GPIO */
    write_sysfs("/sys/class/gpio/export", GPIO_PIN);

    sleep(1); // allow sysfs to create files

    /* Set direction */
    write_sysfs("/sys/class/gpio/gpio17/direction", "out");

    while (1) {
        /* LED ON */
        write_sysfs("/sys/class/gpio/gpio17/value", "1");
        sleep(1);

        /* LED OFF */
        write_sysfs("/sys/class/gpio/gpio17/value", "0");
        sleep(1);
    }

    /* Unexport (never reached here) */
    write_sysfs("/sys/class/gpio/unexport", GPIO_PIN);

    return 0;
}
