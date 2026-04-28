// ili922x_test.c - ILI9225 SPI test (libgpiod v2 compatible)

#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <gpiod.h>
#include <linux/spi/spidev.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

static struct gpiod_chip *chip;
static struct gpiod_line_request *req;
static int dc_offset, rst_offset, bl_offset;

static void die(const char *msg) {
    perror(msg);
    exit(1);
}

static void msleep(unsigned ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (long)(ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

static void gpio_set(int offset, int value) {
    if (gpiod_line_request_set_value(req, offset, value) < 0)
        die("gpiod_line_request_set_value");
}

// ---------- SPI ----------
static int spi_open(const char *dev, uint32_t hz) {
    int fd = open(dev, O_RDWR);
    if (fd < 0) die("open spidev");

    uint8_t mode = SPI_MODE_0; // try MODE_3 if needed
    uint8_t bits = 8;

    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) die("SPI_IOC_WR_MODE");
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) die("SPI_IOC_WR_BITS_PER_WORD");
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &hz) < 0) die("SPI_IOC_WR_MAX_SPEED_HZ");

    return fd;
}

static void spi_write_bytes(int spi_fd, const uint8_t *tx, size_t n) {
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .len = (uint32_t)n,
    };
    if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr) < 0)
        die("SPI_IOC_MESSAGE");
}

static void lcd_write_reg16(int spi_fd, uint16_t reg, uint16_t val) {
    uint8_t b[2];

    gpio_set(dc_offset, 0);
    b[0] = reg >> 8;
    b[1] = reg & 0xFF;
    spi_write_bytes(spi_fd, b, 2);

    gpio_set(dc_offset, 1);
    b[0] = val >> 8;
    b[1] = val & 0xFF;
    spi_write_bytes(spi_fd, b, 2);
}

static void lcd_set_window(int spi_fd, int x0, int y0, int x1, int y1) {
    lcd_write_reg16(spi_fd, 0x0036, x1);
    lcd_write_reg16(spi_fd, 0x0037, x0);
    lcd_write_reg16(spi_fd, 0x0038, y1);
    lcd_write_reg16(spi_fd, 0x0039, y0);
    lcd_write_reg16(spi_fd, 0x0020, x0);
    lcd_write_reg16(spi_fd, 0x0021, y0);
}

static void lcd_write_gram_stream(int spi_fd, const uint16_t *pix, size_t count) {
    lcd_write_reg16(spi_fd, 0x0022, 0x0000);

    gpio_set(dc_offset, 1);

    const size_t CHUNK = 256;
    uint8_t buf[CHUNK * 2];

    size_t i = 0;
    while (i < count) {
        size_t n = (count - i > CHUNK) ? CHUNK : (count - i);
        for (size_t k = 0; k < n; k++) {
            uint16_t v = pix[i + k];
            buf[2*k]     = v >> 8;
            buf[2*k + 1] = v & 0xFF;
        }
        spi_write_bytes(spi_fd, buf, n * 2);
        i += n;
    }
}

static void lcd_reset(void) {
    gpio_set(rst_offset, 1);
    msleep(5);
    gpio_set(rst_offset, 0);
    msleep(20);
    gpio_set(rst_offset, 1);
    msleep(50);
}

static void lcd_init_ili922x(int spi_fd) {
    lcd_reset();

    lcd_write_reg16(spi_fd, 0x0001, 0x011C);
    lcd_write_reg16(spi_fd, 0x0002, 0x0100);
    lcd_write_reg16(spi_fd, 0x0003, 0x1030);
    lcd_write_reg16(spi_fd, 0x0008, 0x0808);
    lcd_write_reg16(spi_fd, 0x000C, 0x0000);
    lcd_write_reg16(spi_fd, 0x000F, 0x0B01);

    lcd_write_reg16(spi_fd, 0x0010, 0x0A00);
    lcd_write_reg16(spi_fd, 0x0011, 0x1038);
    msleep(50);
    lcd_write_reg16(spi_fd, 0x0012, 0x1121);
    lcd_write_reg16(spi_fd, 0x0013, 0x0063);
    lcd_write_reg16(spi_fd, 0x0014, 0x5A00);
    msleep(50);

    lcd_set_window(spi_fd, 0, 0, 175, 219);
    lcd_write_reg16(spi_fd, 0x0007, 0x1017);
    msleep(20);
}

static void lcd_fill(int spi_fd, uint16_t color) {
    const int W = 176, H = 220;
    lcd_set_window(spi_fd, 0, 0, W-1, H-1);

    uint16_t line[176];
    for (int i = 0; i < W; i++)
        line[i] = color;

    for (int y = 0; y < H; y++)
        lcd_write_gram_stream(spi_fd, line, W);
}

int main(int argc, char **argv) {
    const char *spidev = (argc > 1) ? argv[1] : "/dev/spidev0.0";
    uint32_t hz = (argc > 2) ? strtoul(argv[2], NULL, 10) : 8000000;

    dc_offset  = (argc > 3) ? atoi(argv[3]) : 25;
    rst_offset = (argc > 4) ? atoi(argv[4]) : 24;
    bl_offset  = (argc > 5) ? atoi(argv[5]) : 18;

    printf("SPI=%s speed=%u DC=%d RST=%d BL=%d\n",
           spidev, hz, dc_offset, rst_offset, bl_offset);

    chip = gpiod_chip_open("/dev/gpiochip0");
    if (!chip) die("gpiod_chip_open");

    unsigned int offsets[3] = { dc_offset, rst_offset, bl_offset };

    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);

    struct gpiod_line_config *config = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(config, offsets, 3, settings);

    req = gpiod_chip_request_lines(chip, NULL, config);
    if (!req) die("gpiod_chip_request_lines");

    gpio_set(bl_offset, 1);

    int spi_fd = spi_open(spidev, hz);
    lcd_init_ili922x(spi_fd);

    lcd_fill(spi_fd, 0xF800); msleep(600);
    lcd_fill(spi_fd, 0x07E0); msleep(600);
    lcd_fill(spi_fd, 0x001F); msleep(600);
    lcd_fill(spi_fd, 0xFFFF); msleep(600);

    close(spi_fd);
    gpiod_line_request_release(req);
    gpiod_chip_close(chip);

    printf("Done.\n");
    return 0;
}

