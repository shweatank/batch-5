#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Mutex declaration */
pthread_mutex_t lock;

void *print_message_function(void *ptr);

int main()
{
    pthread_t thread1, thread2;
    char *message1 = "Thread 1";
    char *message2 = "Thread 2";
    int iret1, iret2;

    /* Initialize the mutex */
    pthread_mutex_init(&lock, NULL);

    /* Create threads */
    iret1 = pthread_create(&thread1, NULL, print_message_function, (void*)message1);
    iret2 = pthread_create(&thread2, NULL, print_message_function, (void*)message2);

    /* Wait for threads to finish */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Thread 1 returns: %d\n", iret1);
    printf("Thread 2 returns: %d\n", iret2);

    /* Destroy the mutex */
    pthread_mutex_destroy(&lock);

    exit(0);
}

void *print_message_function(void *ptr)
{
    char *message = (char *)ptr;

    /* Lock before critical section */
    pthread_mutex_lock(&lock);

    printf("%s\n", message);

    /* Unlock after critical section */
    pthread_mutex_unlock(&lock);

    return NULL;
}

