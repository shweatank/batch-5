#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

#define WIDTH  176
#define HEIGHT 220

#define COLOR_WHITE 0xFFFF
#define COLOR_BLACK 0x0000

/* 8x8 bitmap for letter 'A' */
uint8_t font_A[8] = {
    0x18,  //   XX
    0x24,  //  X  X
    0x42,  // X    X
    0x42,  // X    X
    0x7E,  // XXXXXX
    0x42,  // X    X
    0x42,  // X    X
    0x00
};

int main()
{
    int fb = open("/dev/fb0", O_RDWR);
    if (fb < 0) {
        perror("Framebuffer open failed");
        return -1;
    }

    uint16_t *fbp = (uint16_t *)mmap(
        NULL,
        WIDTH * HEIGHT * 2,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fb,
        0
    );

    if (fbp == MAP_FAILED) {
        perror("mmap failed");
        close(fb);
        return -1;
    }

    /* Clear screen */
    for (int i = 0; i < WIDTH * HEIGHT; i++)
        fbp[i] = COLOR_BLACK;

    int start_x = 80;
    int start_y = 100;

    /* Draw 'A' */
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (font_A[row] & (1 << (7 - col))) {
                int x = start_x + col;
                int y = start_y + row;
                fbp[y * WIDTH + x] = COLOR_WHITE;
            }
        }
    }

    munmap(fbp, WIDTH * HEIGHT * 2);
    close(fb);

    return 0;
}

