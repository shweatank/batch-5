#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>

// ---------------- GPIO helpers ----------------
void gpio_export(int pin){
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    if(fd>=0){ dprintf(fd,"%d",pin); close(fd); }
}
void gpio_direction(int pin,const char* dir){
    char path[50]; sprintf(path,"/sys/class/gpio/gpio%d/direction",pin);
    int fd=open(path,O_WRONLY); if(fd>=0){ write(fd,dir,strlen(dir)); close(fd);}
}
void gpio_write(int pin,int value){
    char path[50]; sprintf(path,"/sys/class/gpio/gpio%d/value",pin);
    int fd=open(path,O_WRONLY); if(fd>=0){ dprintf(fd,"%d",value); close(fd);}
}

// ---------------- Pin Definitions ----------------
#define DC    24  // RS pin
#define RESET 25
#define SPI_DEVICE "/dev/spidev0.1" // CE1
#define SPI_SPEED 20000000
int spi_fd;

// ---------------- SPI helpers ----------------
void spi_write(uint8_t data){ write(spi_fd,&data,1);}
void write_command(uint8_t cmd){ gpio_write(DC,0); spi_write(cmd);}
void write_data(uint8_t data){ gpio_write(DC,1); spi_write(data);}

// ---------------- ILI9225 full initialization ----------------
void reset_display(){
    gpio_write(RESET,0); usleep(100000);
    gpio_write(RESET,1); usleep(100000);
}

void init_display(){
    reset_display();

    write_command(0x01); // Software reset
    usleep(50000);

    write_command(0x28); // Display OFF

    // Full working minimal initialization
    write_command(0x10); write_data(0x0000);
    write_command(0x11); write_data(0x0001);
    write_command(0x12); write_data(0x0006);
    write_command(0x13); write_data(0x0000);
    write_command(0x20); write_data(0x0000);
    write_command(0x21); write_data(0x0000);

    write_command(0x30); write_data(0x0000);
    write_command(0x31); write_data(0x00AF);
    write_command(0x32); write_data(0x0000);
    write_command(0x33); write_data(0x00DB);

    write_command(0x34); write_data(0x00DB);
    write_command(0x35); write_data(0x0000);

    write_command(0x36); write_data(0x00AF);
    write_command(0x37); write_data(0x0000);
    write_command(0x38); write_data(0x0000);
    write_command(0x39); write_data(0x00AF);

    write_command(0x07); // Display ON
    usleep(100000);
}

// ---------------- GRAM helper ----------------
void set_pixel(uint16_t x,uint16_t y,uint16_t color){
    write_command(0x30); write_data(x);
    write_command(0x31); write_data(x);
    write_command(0x32); write_data(y);
    write_command(0x33); write_data(y);
    write_command(0x34); // start GRAM write
    write_data(color >> 8);
    write_data(color & 0xFF);
}

uint16_t color565(uint8_t r,uint8_t g,uint8_t b){
    return ((r&0xF8)<<8)|((g&0xFC)<<3)|(b>>3);
}

// ---------------- Simple 5x8 font ----------------
uint8_t font5x8[96][5] = {
    // Only 'A' to 'Z' for simplicity
    {0x7C,0x12,0x12,0x7C,0x00}, // A
    {0x7E,0x4A,0x4A,0x34,0x00}, // B
    {0x3C,0x42,0x42,0x24,0x00}, // C
    // ... add more characters if needed
};

// Draw single character
void draw_char(char c,uint16_t x,uint16_t y,uint16_t color){
    if(c<'A'||c>'Z') return;
    uint8_t *ch = font5x8[c-'A'];
    for(int i=0;i<5;i++){
        uint8_t col = ch[i];
        for(int j=0;j<8;j++){
            if(col & (1<<j)) set_pixel(x+i,y+j,color);
        }
    }
}

// Draw string horizontally
void draw_string(const char *str,uint16_t x,uint16_t y,uint16_t color){
    while(*str){
        draw_char(*str,x,y,color);
        x += 6; // 5 pixel width +1 space
        str++;
    }
}

// ---------------- Main ----------------
int main(){
    // Setup GPIO
    gpio_export(DC); gpio_export(RESET);
    usleep(100000);
    gpio_direction(DC,"out");
    gpio_direction(RESET,"out");

    // Setup SPI
    spi_fd = open(SPI_DEVICE,O_RDWR);
    if(spi_fd<0){ perror("SPI open failed"); return 1; }
    uint8_t mode=SPI_MODE_0,bits=8; uint32_t speed=SPI_SPEED;
    ioctl(spi_fd,SPI_IOC_WR_MODE,&mode);
    ioctl(spi_fd,SPI_IOC_WR_BITS_PER_WORD,&bits);
    ioctl(spi_fd,SPI_IOC_WR_MAX_SPEED_HZ,&speed);

    // Initialize display
    init_display();
    printf("Display initialized!\n");

    // Draw some test strings
    draw_string("ABC",10,10,color565(255,0,0));   // Red text
    draw_string("HELLO",10,30,color565(0,255,0)); // Green text
    draw_string("WORLD",10,50,color565(0,0,255)); // Blue text
    printf("Test strings drawn!\n");

    close(spi_fd);
    return 0;
}
