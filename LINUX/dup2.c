#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
int main()
{
	//int newfd=dup(1);
	//printf("newfd=%d\n",newfd);
	int newfd=dup2(1,9);
	printf("newfd=%d\n",newfd);
	write(1,"hi",2);
	write(9,"hello",5);
}
