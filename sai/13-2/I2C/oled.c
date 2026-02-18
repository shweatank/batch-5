#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h>

#define I2C_DEV "/dev/i2c-1"
#define OLED_ADDR 0x3C   // Change to 0x3D if needed

int fd;

/* Send command */
void oled_cmd(unsigned char cmd)
{
    unsigned char buffer[2];
    buffer[0] = 0x00;   // Command mode
    buffer[1] = cmd;
    write(fd, buffer, 2);
}

/* Send data */
void oled_data(unsigned char data)
{
    unsigned char buffer[2];
    buffer[0] = 0x40;   // Data mode
    buffer[1] = data;
    write(fd, buffer, 2);
}

/* Basic SSD1306 init */
void oled_init()
{
    oled_cmd(0xAE); // Display OFF
    oled_cmd(0x20); // Memory mode
    oled_cmd(0x00); // Horizontal addressing
    oled_cmd(0xB0);
    oled_cmd(0xC8);
    oled_cmd(0x00);
    oled_cmd(0x10);
    oled_cmd(0x40);
    oled_cmd(0x81);
    oled_cmd(0xFF);
    oled_cmd(0xA1);
    oled_cmd(0xA6);
    oled_cmd(0xA8);
    oled_cmd(0x3F);
    oled_cmd(0xA4);
    oled_cmd(0xD3);
    oled_cmd(0x00);
    oled_cmd(0xD5);
    oled_cmd(0xF0);
    oled_cmd(0xD9);
    oled_cmd(0x22);
    oled_cmd(0xDA);
    oled_cmd(0x12);
    oled_cmd(0xDB);
    oled_cmd(0x20);
    oled_cmd(0x8D);
    oled_cmd(0x14);
    oled_cmd(0xAF); // Display ON
}

/* Fill screen */
void oled_fill()
{
    for(int page = 0; page < 8; page++)
    {
        oled_cmd(0xB0 + page);
        oled_cmd(0x00);
        oled_cmd(0x10);

        for(int col = 0; col < 128; col++)
            oled_data(0xFF);   // All pixels ON
    }
}

int main()
{
    fd = open(I2C_DEV, O_RDWR);
    if(fd < 0)
    {
        perror("Failed to open I2C device");
        return -1;
    }

    if(ioctl(fd, I2C_SLAVE, OLED_ADDR) < 0)
    {
        perror("Failed to set I2C address");
        return -1;
    }

    oled_init();
    oled_fill();

    printf("If OLED turns fully white, it is working.\n");
    oled_cmd(0xAe);

    close(fd);
    return 0;
}
