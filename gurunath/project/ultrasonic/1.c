#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

#define TRIG 17  // WiringPi pin 0 = BCM 17
#define ECHO 27 // WiringPi pin 2 = BCM 27

// Function to get microseconds timestamp
long get_time_us() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000000L) + tv.tv_usec;
}

int main(void) {
    long start_time, end_time, travel_time;
    double distance_cm;

    // Initialize WiringPi
    if (wiringPiSetup() == -1) {
        printf("WiringPi setup failed!\n");
        return 1;
    }

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    while (1) {
        // Send 10µs trigger pulse
        digitalWrite(TRIG, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);

        // Wait for ECHO high
        start_time = get_time_us();
        long timeout = start_time + 100000; // 100ms timeout
        while (digitalRead(ECHO) == LOW && get_time_us() < timeout);

        // Start timing
        start_time = get_time_us();

        // Wait for ECHO low
        while (digitalRead(ECHO) == HIGH && get_time_us() < timeout);
        end_time = get_time_us();

        travel_time = end_time - start_time;

        // Calculate distance in cm
        distance_cm = (travel_time * 0.0343) / 2.0;

        printf("Distance: %.1f cm\n", distance_cm);

        delay(200); // 200ms between measurements
    }

    return 0;
}

