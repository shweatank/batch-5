#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
        printf("In parent\n");
        int ret = fork();
        if(fork == 0) {
                printf("In child\n");
                execl("./main","main",NULL);
                exit(0);
        }
        wait(NULL);
}

