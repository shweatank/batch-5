#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <string.h>

// ---------- GPIO helpers ----------
void gpio_export(int pin){
    int fd=open("/sys/class/gpio/export", O_WRONLY);
    if(fd>=0){ dprintf(fd,"%d",pin); close(fd);}
}
void gpio_direction(int pin,const char* dir){
    char path[50]; sprintf(path,"/sys/class/gpio/gpio%d/direction",pin);
    int fd=open(path,O_WRONLY); if(fd>=0){ write(fd,dir,strlen(dir)); close(fd);}
}
void gpio_write(int pin,int val){
    char path[50]; sprintf(path,"/sys/class/gpio/gpio%d/value",pin);
    int fd=open(path,O_WRONLY); if(fd>=0){ dprintf(fd,"%d",val); close(fd);}
}

// ---------- Pin definitions ----------
#define DC    24
#define RESET 25
#define SPI_DEVICE "/dev/spidev0.1"  // CE1
#define SPI_SPEED 20000000

int spi_fd;

// ---------- SPI helpers ----------
void spi_write(uint8_t data){ write(spi_fd,&data,1);}
void write_command(uint8_t cmd){ gpio_write(DC,0); spi_write(cmd);}
void write_data(uint8_t data){ gpio_write(DC,1); spi_write(data);}

// ---------- ILI9225 init ----------
void reset_display(){
    gpio_write(RESET,0); usleep(100000);
    gpio_write(RESET,1); usleep(100000);
}

void init_display(){
    reset_display();
    write_command(0x01); usleep(50000); // software reset
    write_command(0x28); // display off
    // minimal initialization
    write_command(0x10); write_data(0x00);
    write_command(0x11); write_data(0x01);
    write_command(0x12); write_data(0x06);
    write_command(0x13); write_data(0x00);
    write_command(0x07); // display on
    usleep(100000);
}

// ---------- Pixel helper ----------
void set_pixel(uint16_t x,uint16_t y,uint16_t color){
    write_command(0x30); write_data(x);
    write_command(0x31); write_data(x);
    write_command(0x32); write_data(y);
    write_command(0x33); write_data(y);
    write_command(0x34);
    write_data(color>>8);
    write_data(color&0xFF);
}

uint16_t color565(uint8_t r,uint8_t g,uint8_t b){
    return ((r&0xF8)<<8)|((g&0xFC)<<3)|(b>>3);
}

// ---------- Fill screen ----------
void fill_screen(uint16_t color){
    int width=176, height=220; // typical ILI9225 resolution
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            set_pixel(x,y,color);
        }
    }
}

// ---------- Main ----------
int main(){
    // GPIO setup
    gpio_export(DC); gpio_export(RESET);
    usleep(100000);
    gpio_direction(DC,"out");
    gpio_direction(RESET,"out");

    // SPI setup
    spi_fd=open(SPI_DEVICE,O_RDWR);
    if(spi_fd<0){ perror("SPI open failed"); return 1;}
    uint8_t mode=SPI_MODE_0,bits=8; uint32_t speed=SPI_SPEED;
    ioctl(spi_fd,SPI_IOC_WR_MODE,&mode);
    ioctl(spi_fd,SPI_IOC_WR_BITS_PER_WORD,&bits);
    ioctl(spi_fd,SPI_IOC_WR_MAX_SPEED_HZ,&speed);

    init_display();
    printf("Display initialized!\n");

    // Fill screen with red → green → blue
    printf("Filling red...\n");
    fill_screen(color565(255,0,0)); sleep(1);
    printf("Filling green...\n");
    fill_screen(color565(0,255,0)); sleep(1);
    printf("Filling blue...\n");
    fill_screen(color565(0,0,255)); sleep(1);

    close(spi_fd);
    printf("Test complete.\n");
    return 0;
}
