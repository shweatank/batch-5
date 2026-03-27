#include <stdio.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define key 1

void main()
{
    int semid = semget(key, 2, IPC_CREAT | 0664);
    if (semid < 0)
    {
        perror("semid");
        return;
    }

    semctl(semid, 1, SETVAL, 1);

    if (fork() == 0)
    {
        char ch = 'A';
        while (ch != 'Z' + 1)
        {
            struct sembuf v = {0, 0, 0};
            semop(semid, &v, 1);
            printf("%c", ch++);
            //fflush(stdout);
            semctl(semid, 0, SETVAL, 1);
            semctl(semid, 1, SETVAL, 0);
        }
        exit(0);
    }
    else
    {
        char ch = 'a';
        while (ch != 'z' + 1)
        {
            struct sembuf v = {1, 0, 0};
            semop(semid, &v, 1);
            printf("%c", ch++);
           // fflush(stdout); 
            semctl(semid, 1, SETVAL, 1);
            semctl(semid, 0, SETVAL, 0);
        }
    }
    wait(0);
}
