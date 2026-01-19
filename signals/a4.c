#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

/* SIGHUP handler */
void sighup_handler(int sig)
{
    printf("Child: SIGHUP received (parent exited)\n");
}

int main()
{
    pid_t pid;
    struct sigaction sa;

    pid = fork();

    if (pid == 0) {  
        /* -------- CHILD -------- */

        /* Install SIGHUP handler */
        sa.sa_handler = sighup_handler;
        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGHUP, &sa, NULL);

        printf("Child PID = %d, waiting for SIGHUP...\n", getpid());

        pause();   // Wait for SIGHUP

        printf("Child: exiting after SIGHUP\n");
        exit(0);

    } else {  
        /* -------- PARENT -------- */

        printf("Parent PID = %d, exiting now...\n", getpid());
        sleep(2);   // Give child time to pause
        exit(0);    // Parent exits → kernel sends SIGHUP
    }
}

