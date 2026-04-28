#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>

#define LED 21  // WiringPi pin 0 = BCM 17 (physical pin 11)

int main(void)
{
    // Initialize WiringPi
    if (wiringPiSetup() == -1) {
        printf("WiringPi setup failed!\n");
        return 1;
    }

    pinMode(LED, OUTPUT); // Set pin as output

    while (1) {
        digitalWrite(LED, HIGH); // LED ON
        printf("LED ON\n");
        sleep(1);                // Wait 1 second

        digitalWrite(LED, LOW);  // LED OFF
        printf("LED OFF\n");
        sleep(1);                // Wait 1 second
    }

    return 0;
}

