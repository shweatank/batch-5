#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s <main_pid> <a> <b>\n", argv[0]);
        return 1;
    }

    pid_t main_pid = atoi(argv[1]);
    int a = atoi(argv[2]);
    int b = atoi(argv[3]);

    if (b == 0)
    {
        printf("Error: Division by zero\n");
        return 1;
    }

    int result = a / b;

    union sigval val;
    val.sival_int = result;

    if (sigqueue(main_pid, SIGRTMIN + 1, val) == -1)
    {
        perror("sigqueue");
        return 1;
    }

    return 0;
}

