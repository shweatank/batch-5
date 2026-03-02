#include <stdio.h>
#include <wiringPi.h>
#include <sys/time.h>

#define TRIG 17
#define ECHO 27

long getTimeMicroseconds() {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * 1000000 + currentTime.tv_usec;
}

int main() {

    wiringPiSetupGpio();  // Use BCM GPIO numbering

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    while (1) {

        // Send 10us pulse
        digitalWrite(TRIG, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);

        // Wait for echo start
        while (digitalRead(ECHO) == LOW);

        long startTime = getTimeMicroseconds();

        // Wait for echo end
        while (digitalRead(ECHO) == HIGH);

        long endTime = getTimeMicroseconds();

        long duration = endTime - startTime;

        float distance = duration * 0.0343 / 2;

        printf("Distance: %.2f cm\n", distance);

        delay(500);
    }

    return 0;
}
