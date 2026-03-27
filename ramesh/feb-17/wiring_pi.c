#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <unistd.h>

#define SPI_CHANNEL 0
#define SPI_SPEED 16000000

#define DC 5      // WiringPi pin for GPIO24
#define RESET 6   // WiringPi pin for GPIO25

// Set DC pin
void setDC(int value) {
    digitalWrite(DC, value);
}

// Hardware reset
void resetDisplay() {
    digitalWrite(RESET, LOW);
    usleep(100000); // 100 ms
    digitalWrite(RESET, HIGH);
    usleep(100000); // 100 ms
}

// SPI write byte
void spiWrite(uint8_t byte, int dc) {
    setDC(dc); // 0=command, 1=data
    wiringPiSPIDataRW(SPI_CHANNEL, &byte, 1);
}

// Send command + optional data
void sendCommand(uint8_t cmd, uint8_t *data, int len) {
    spiWrite(cmd, 0); // command
    for (int i = 0; i < len; i++)
        spiWrite(data[i], 1);
}

// Initialize ILI9295
void iliInit() {
    resetDisplay();

    // Software reset
    sendCommand(0x01, NULL, 0);
    usleep(5000);

    // Display ON
    sendCommand(0x29, NULL, 0);

    // Optional: add more init commands (power, gamma, pixel format)
}

// Fill screen with a color (RGB565)
void fillScreen(uint16_t color) {
    uint8_t col_addr[] = {0x00, 0x00, 0x00, 0xEF}; // 0-239
    uint8_t page_addr[] = {0x00, 0x00, 0x01, 0x3F}; // 0-319
    sendCommand(0x2A, col_addr, 4);
    sendCommand(0x2B, page_addr, 4);
    sendCommand(0x2C, NULL, 0); // memory write

    int total_pixels = 240 * 320;
    uint8_t high = color >> 8;
    uint8_t low = color & 0xFF;

    for (int i = 0; i < total_pixels; i++) {
        spiWrite(high, 1);
        spiWrite(low, 1);
    }
}

int main() {
    // Initialize WiringPi
    wiringPiSetup();

    pinMode(DC, OUTPUT);
    pinMode(RESET, OUTPUT);

    // Initialize SPI channel 0
    if (wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) < 0) {
        printf("SPI Setup failed\n");
        return -1;
    }

    // Initialize display
    iliInit();

    // Fill screen with green (RGB565: 0x07E0)
    fillScreen(0x07E0);

    return 0;
}

