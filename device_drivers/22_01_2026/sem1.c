#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<sys/shm.h>


int main(){
	
	char ch='a';
	int id=shmget(1,8,IPC_CREAT);
	/*int *p=shmat(id,NULL,NULL);
	sem_init(p,1,1);
	sem_init(p+1,1,0);*/

	if(fork()==0){
		int *p=shmat(id,NULL,NULL);
		sem_init(p,1,1);
		sem_init(p+1,1,0);
		int ch='a';
		while(ch<='z'){
			sem_wait(p);
			printf("%c ",ch);
			ch++;
			sem_post(p+1);
		}
	}
	else{
		int *p=shmat(id,NULL,NULL);
		sem_init(p,1,1);
		sem_init(p+1,1,0);
		int ch='A';
		while(ch<='Z'){
			sem_wait(p);
			printf("%c ",ch);
			ch++;
			sem_post(p+1);
		}
		
	}

}

