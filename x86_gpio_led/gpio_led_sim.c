#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

bool state = false;
volatile sig_atomic_t stop = 0;

// Handle Ctrl+C to exit cleanly
void handle_sigint(int sig) {
    stop = 1;
}

int main() {
    signal(SIGINT, handle_sigint);
    printf("Starting x86 GPIO LED simulation. Press Ctrl+C to stop.\n");

    while (!stop) {
        state = !state;
        printf("LED %s\n", state ? "ON" : "OFF");
        fflush(stdout); // ensure output shows immediately
        sleep(1);       // blink every second
    }

    printf("Exiting simulation. LED OFF\n");
    return 0;
}
