#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>

struct ili9225_pixel { int x; int y; uint16_t color; };

#define ILI9225_MAGIC 'L'
#define ILI9225_FILL _IOW(ILI9225_MAGIC,0,unsigned short)
#define ILI9225_DRAW_PIXEL _IOW(ILI9225_MAGIC,1,struct ili9225_pixel)

int main()
{
    int fd = open("/dev/ili9225", O_RDWR);
    unsigned short c = 0xF800;
    ioctl(fd, ILI9225_FILL, &c);

    struct ili9225_pixel p = {10,10,0x001F};
    ioctl(fd, ILI9225_DRAW_PIXEL, &p);

    close(fd);
    return 0;
}

