#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
int x=10,y=5;
void add(int n) {
    printf("ADDprocess :%d\n",x + y);
}
void sub(int n1) {
    printf("SUBprocess : %d\n",x-y);
}
void mul(int n2) {
    printf("MULprocess :%d\n",x*y);
}
void divi(int n3) {
    printf("DIVprocess : %d\n",x/y);
}
int main() {
pid_t p1, p2, p3, p4;
    p1 = fork();
    if (p1 == 0) {
        signal(SIGUSR1, add);
        pause();       
       exit(0);
    }
     p2 = fork();
    if (p2 == 0) {
        signal(SIGUSR1, sub);
        pause();
        exit(0);
    }
    p3 = fork();
    if (p3 == 0) {
        signal(SIGUSR1, mul);
        pause();
        exit(0);
    }
    p4 = fork();
    if (p4 == 0) {
        signal(SIGUSR1, divi);
        pause();
        exit(0);
    }
    kill(p1, SIGUSR1);
    sleep(1);
    kill(p2, SIGUSR1);
    sleep(1);
    kill(p3, SIGUSR1);
    sleep(1);
    kill(p4, SIGUSR1);
    sleep(1);
    return 0;
}


