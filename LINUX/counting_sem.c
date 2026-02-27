#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>
int var=7;
#define THREADS 5
#define RESOURCES 3   // Number of printers (available resources)

sem_t sem;  // counting semaphore

void* thread_func(void *arg)
{
    int id = *(int*)arg;

    printf("Thread %d is waiting for a resource\n", id);
    sem_wait(&sem);   // acquire a resource

    printf("Thread %d acquired resource\n", id);
    //sleep(2);         // simulate using the resource

    printf("Thread %d releasing resource\n", id);
    sem_post(&sem);   // release the resource

    return NULL;
}

int main()
{
    pthread_t threads[THREADS];
    int ids[THREADS];

    // Initialize counting semaphore with 3 resources
    sem_init(&sem, 0, RESOURCES);

    // Create threads
    for(int i = 0; i < THREADS; i++)
    {
        ids[i] = i+1;
        pthread_create(&threads[i], NULL, thread_func, &ids[i]);
    }

    // Join threads
    for(int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL);

    sem_destroy(&sem);
    return 0;
}
