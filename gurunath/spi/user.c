#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <stdint.h>

#define SPI_DEV "/dev/spidev0.1"

int main() {
    int fd;
    uint8_t mode = 0;
    uint8_t bits = 8;
    uint32_t speed = 1000000; // 1 MHz safe for SD init

    fd = open(SPI_DEV, O_RDWR);
    if (fd < 0) {
        perror("Cannot open SPI device");
        return 1;
    }

    ioctl(fd, SPI_IOC_WR_MODE, &mode);
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

    // Send 80 clock cycles with CS high
    uint8_t dummy[10];
    for(int i=0;i<10;i++) dummy[i] = 0xFF;
    write(fd, dummy, 10);

    // Prepare CMD0 (reset command)
    uint8_t cmd[6];
    cmd[0] = 0x40 | 0; // CMD0
    cmd[1] = 0;        // arg[31:24]
    cmd[2] = 0;        // arg[23:16]
    cmd[3] = 0;        // arg[15:8]
    cmd[4] = 0;        // arg[7:0]
    cmd[5] = 0x95;     // CRC valid for CMD0

    write(fd, cmd, 6);

    // Read response (max 8 bytes)
    uint8_t resp = 0xFF;
    int timeout = 8;
    for (int i = 0; i < timeout; i++) {
        write(fd, (uint8_t[]){0xFF}, 1);  // clock byte
        read(fd, &resp, 1);
        if (resp != 0xFF) break;
    }

    if (resp != 0xFF)
        printf("✅ SD card responded to CMD0: 0x%02X\n", resp);
    else
        printf("❌ No response from SD card. Check wiring, power, or CS.\n");

    close(fd);
    return 0;
}

