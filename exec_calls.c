#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;

    printf("Parent started\n");
    printf("Parent PID: %d\n", getpid());

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        /* Child process */
        printf("Child before exec\n");
        printf("Child PID: %d\n", getpid());

        /* Replace child process image with another ELF */
        execl("./p_calculate", "p_calculate", "Hello_from_parent", NULL);

        /* exec returns only on failure */
        perror("execl failed");
        exit(EXIT_FAILURE);
    } else {
        /* Parent process */
        int status;
        waitpid(pid, &status, 0);

        printf("Parent: child exited\n");
        if (WIFEXITED(status)) {
            printf("Exit status: %d\n", WEXITSTATUS(status));
        }
    }

    return 0;
}

