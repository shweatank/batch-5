#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_KEY 1234

struct message {
    long msg_type;
    int a, b;
    float add, sub, mul, div;
};

int main() {
    int msgid;
    struct message msg;

    
    msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
    if (msgid < 0) {
        perror("msgget");
        exit(1);
    }

    printf("Main process waiting for results...\n");

    
    msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), 1, 0);

    
    printf("\nReceived Data:\n");
    printf("A = %d, B = %d\n", msg.a, msg.b);
    printf("Addition       : %.2f\n", msg.add);
    printf("Subtraction    : %.2f\n", msg.sub);
    printf("Multiplication : %.2f\n", msg.mul);
    printf("Division       : %.2f\n", msg.div);

   
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}

