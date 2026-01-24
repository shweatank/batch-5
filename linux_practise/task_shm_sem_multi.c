//Write a program where a parent creates multiple child processes. Each child writes data into shared memory, and access is synchronized using System V semaphores.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/shm.h>

int var = 10;
int main() {
	int childs;
	int shmid;
	printf("Enter the no.of childs to be created: ");
	scanf("%d",&childs);
	pid_t pid;
	while(childs) {
		pid = fork();
		if(pid == 0) {
			shmid = shmget(childs,sizeof(var),IPC_CREAT|0666);



