#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;

    pid = fork();  

    if (pid < 0) {
        printf("Fork failed\n");
    }
    else if (pid == 0) {
        
        printf("Child process executing hello program...\n");
        execl("./hello", "hello", NULL);
    }
    else {
        
        printf("Parent process running...\n");
    }

    return 0;
}

