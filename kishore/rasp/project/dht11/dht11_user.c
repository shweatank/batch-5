#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <unistd.h>

#define DHTPIN 4  // GPIO4

// DHT11 timing constants
#define MAX_TIMINGS 85

int data[5] = {0,0,0,0,0};

int read_dht11() {
    int laststate = 1;
    int counter = 0;
    int j = 0, i;

    for(i=0;i<5;i++) data[i] = 0;

    // Send start signal
    gpioSetMode(DHTPIN, PI_OUTPUT);
    gpioWrite(DHTPIN, 0);
    gpioDelay(18000); // 18ms
    gpioWrite(DHTPIN, 1);
    gpioDelay(40);    // 40us
    gpioSetMode(DHTPIN, PI_INPUT);

    // Read the data
    for(i=0;i<MAX_TIMINGS;i++) {
        counter = 0;
        while (gpioRead(DHTPIN) == laststate) {
            counter++;
            gpioDelay(1); // 1us
            if(counter == 255) break;
        }
        laststate = gpioRead(DHTPIN);

        if(i >= 4 && i % 2 == 0) {
            data[j/8] <<= 1;
            if(counter > 16)
                data[j/8] |= 1;
            j++;
        }
    }

    // Check if we got 40 bits and checksum is valid
    if(j >= 40) {
        if(data[4] == ((data[0]+data[1]+data[2]+data[3]) & 0xFF))
            return 1;
        else
            return 0;
    }
    return 0;
}

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio init failed\n");
        return 1;
    }

    while(1) {
        if(read_dht11()) {
            printf("Humidity = %d.%d %% Temperature = %d.%d *C\n",
                data[0], data[1], data[2], data[3]);
        } else {
            printf("Failed to read from DHT11\n");
        }
        sleep(1);
    }

    gpioTerminate();
    return 0;
}

