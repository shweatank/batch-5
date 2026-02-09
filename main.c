#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void result_handler(int sig, siginfo_t *info, void *context)
{
    int result = info->si_value.sival_int;
    char buffer[100];
    int len;

    if (sig == SIGUSR1)
        len = snprintf(buffer, sizeof(buffer),
                       "Received ADD result: %d\n", result);
    else if (sig == SIGUSR2)
        len = snprintf(buffer, sizeof(buffer),
                       "Received result: %d\n", result);

    write(STDOUT_FILENO, buffer, len);
}

int main()
{
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = result_handler;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    printf("Main process PID: %d\n", getpid());
    printf("Waiting for results...\n");
    fflush(stdout);

    while (1)
        pause();

    return 0;
}

