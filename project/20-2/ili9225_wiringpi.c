#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define SPI_DEV     "/dev/spidev0.1"
#define SPI_SPEED   10000000  // 10 MHz
#define MAX_STRING  64

// GPIOs for DC and RESET (change according to your wiring)
#define DC_GPIO     24
#define RST_GPIO    25

// ILI9225 commands (basic)
#define CMD_RESET   0x01
#define CMD_DISPLAY_ON  0x29
#define CMD_DISPLAY_OFF 0x28

// Simple GPIO write via sysfs
void gpio_write(int gpio, int value) {
    char path[64];
    char val_str[2];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", gpio);
    int fd = open(path, O_WRONLY);
    if(fd < 0) return;
    snprintf(val_str, sizeof(val_str), "%d", value);
    write(fd, val_str, 1);
    close(fd);
}

// SPI write (DC = 0 for command, 1 for data)
int spi_write(int fd, uint8_t dc, uint8_t *buf, size_t len) {
    struct spi_ioc_transfer tr;
    memset(&tr, 0, sizeof(tr));
    tr.tx_buf = (unsigned long)buf;
    tr.rx_buf = 0;
    tr.len = len;
    tr.speed_hz = SPI_SPEED;
    tr.bits_per_word = 8;
    
    gpio_write(DC_GPIO, dc);
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1){
        perror("SPI write failed");
        return -1;
    }
    return 0;
}

// Simple 5x7 font (subset)
const uint8_t font5x7[95][5] = {
    // only implement for ASCII 32..126
    {0x00,0x00,0x00,0x00,0x00}, // space
    {0x3E,0x51,0x49,0x45,0x3E}, // 0
    {0x00,0x42,0x7F,0x40,0x00}, // 1
    {0x42,0x61,0x51,0x49,0x46}, // 2
    // ... fill rest as needed
};

// Send command
void ili_cmd(int fd, uint8_t cmd) {
    spi_write(fd, 0, &cmd, 1);
}

// Send data
void ili_data(int fd, uint8_t *data, size_t len) {
    spi_write(fd, 1, data, len);
}

// Reset the display
void ili_reset() {
    gpio_write(RST_GPIO, 0);
    usleep(100000);
    gpio_write(RST_GPIO, 1);
    usleep(100000);
}

// Clear display (black)
void ili_clear(int fd) {
    uint8_t buf[240*2]; // width*2 bytes per pixel
    memset(buf, 0x00, sizeof(buf));
    for(int y=0; y<320; y++){
        ili_cmd(fd, 0x2C); // memory write
        ili_data(fd, buf, 240*2);
    }
}

// Draw string at top-left (very simple, no wrapping)
void ili_draw_string(int fd, const char *str) {
    uint8_t buf[6];
    while(*str) {
        char c = *str++;
        if(c<32 || c>126) c=' ';
        memcpy(buf, font5x7[c-32], 5);
        buf[5] = 0x00; // space
        ili_data(fd, buf, 6);
    }
}

int main() {
    int spi_fd = open(SPI_DEV, O_RDWR);
    if(spi_fd < 0) {
        perror("Cannot open SPI device");
        return 1;
    }

    // SPI mode
    uint8_t mode = SPI_MODE_0;
    ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);

    ili_reset();
    ili_clear(spi_fd);

    // Main loop: read string from /dev/mouse_touch
    int fd_chardev = open("/dev/mouse_touch", O_RDONLY);
    if(fd_chardev < 0) {
        perror("Cannot open /dev/mouse_touch");
        return 1;
    }

    char buf[MAX_STRING];
    while(1){
        ssize_t n = read(fd_chardev, buf, MAX_STRING-1);
        if(n > 0){
            buf[n] = '\0';
            printf("Display string: %s\n", buf);
            ili_clear(spi_fd);
            ili_draw_string(spi_fd, buf);
        }
        usleep(200000); // poll every 200ms
    }

    close(fd_chardev);
    close(spi_fd);
    return 0;
}
