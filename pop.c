#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

#define MSG_KEY 1234

struct message {
    long msg_type;
    int a, b;
    float add, sub, mul, div;
};

int x, y;
float r_add, r_sub, r_mul, r_div;


void* add(void* arg) {
    r_add = x + y;
    return NULL;
}

void* sub(void* arg) {
    r_sub = x - y;
    return NULL;
}

void* mul(void* arg) {
    r_mul = x * y;
    return NULL;
}

void* divide(void* arg) {
    if (y != 0)
        r_div = (float)x / y;
    else
        r_div = 0;
    return NULL;
}

int main() {
    int msgid;
    struct message msg;
    pthread_t t1, t2, t3, t4;

    
    msgid = msgget(MSG_KEY, 0666);
    if (msgid < 0) {
        perror("msgget");
        exit(1);
    }

    
    printf("Enter first number: ");
    scanf("%d", &x);

    printf("Enter second number: ");
    scanf("%d", &y);

    
    pthread_create(&t1, NULL, add, NULL);
    pthread_create(&t2, NULL, sub, NULL);
    pthread_create(&t3, NULL, mul, NULL);
    pthread_create(&t4, NULL, divide, NULL);

    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

  
    msg.msg_type = 1;
    msg.a = x;
    msg.b = y;
    msg.add = r_add;
    msg.sub = r_sub;
    msg.mul = r_mul;
    msg.div = r_div;

   
    msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);

    return 0;
}

