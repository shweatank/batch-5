#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>

int main() {
    const char msg[] = "Hello\n";

    for (int i = 0; i < 10; i++) {
        syscall(SYS_write, STDOUT_FILENO, msg, strlen(msg));
    }

    return 0;
}

