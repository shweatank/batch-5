#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>


void printMessage() {
    const char msg[] = "Hello\n";
    syscall(SYS_write, STDOUT_FILENO, msg, strlen(msg));
}


int main() {
    for (int i = 0; i < 10; i++) {
        printMessage();
    }

    return 0;
}

