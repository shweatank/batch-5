#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>

struct ili9225_pixel {
    int x;
    int y;
    unsigned short color;
};

#define ILI9225_MAGIC 'L'
#define ILI9225_FILL      _IOW(ILI9225_MAGIC, 0, unsigned short)
#define ILI9225_DRAW_PIXEL _IOW(ILI9225_MAGIC, 1, struct ili9225_pixel)

int main() {
    int fd = open("/dev/ili9225", O_RDWR);
    if(fd < 0) { perror("open"); return 1; }

    // Fill screen with red
    unsigned short color = 0xF800; // RGB565 red
    if(ioctl(fd, ILI9225_FILL, &color) < 0) {
        perror("ioctl fill"); return 1;
    }
    printf("Screen filled with red\n");

    // Draw blue pixel at (10, 10)
    struct ili9225_pixel px = {10, 10, 0x001F};
    if(ioctl(fd, ILI9225_DRAW_PIXEL, &px) < 0) {
        perror("ioctl pixel"); return 1;
    }
    printf("Pixel drawn at (10,10) with blue\n");

    // Write a horizontal line via write()
    unsigned short line[20];
    for(int i=0;i<20;i++) line[i] = 0x07E0; // green

    // write 20 pixels starting at offset 0
    if(write(fd,line,sizeof(line)) < 0) {
        perror("write"); return 1;
    }
    printf("20 green pixels written at top-left\n");

    close(fd);
    return 0;
}

