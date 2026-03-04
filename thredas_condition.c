#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int flag = 0;  // 0 = thread1, 1 = thread2, 2 = thread3

int ch = 'a';
int ch2 = 'A';
int num = 1;

void* thread1(void* p) {
    while(1) {
        pthread_mutex_lock(&lock);
        while(flag != 0) pthread_cond_wait(&cond, &lock);

        printf("%c\n", ch++);
        if(ch > 'z') ch = 'a';   // wrap around lowercase
        fflush(stdout);

        flag = 1;                // next thread's turn
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);
    }
}

void* thread2(void* p) {
    while(1) {
        pthread_mutex_lock(&lock);
        while(flag != 1) pthread_cond_wait(&cond, &lock);

        printf("%c\n", ch2++);
        if(ch2 > 'Z') ch2 = 'A'; // wrap around uppercase
        fflush(stdout);

        flag = 2;                // next thread's turn
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);
    }
}

void* thread3(void* p) {
    while(1) {
        pthread_mutex_lock(&lock);
        while(flag != 2) pthread_cond_wait(&cond, &lock);

        printf("%d\n", num++);
        fflush(stdout);

        flag = 0;                // next thread's turn
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&lock);
    }
}

int main() {
    pthread_t t1, t2, t3;

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_create(&t3, NULL, thread3, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    return 0;
}
