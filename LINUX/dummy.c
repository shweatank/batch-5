#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
int main()
{
	//int newfd=dup(1);
	//printf("newfd=%d\n",newfd);
	int newfd=dup2(1,2);
	printf("newfd=%d\n",newfd);
	write(1,"hi",2);
	write(2,"hello",5);
	int pi[2];
	if((mkfifo("pro",0664))==-1)
	{
		perror("mkfifo");
		return 0;
	}
}
//if we redirect stderr i.e 2 to stdin i.e 1 we will not see error msg because stdin isn't writable,not because perror() stops working.
