#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main()
{
	int newfd=dup(1);
	printf("new fd=%d\n",newfd);//3
	write(1,"hello",5);
	write(newfd,"sri",3);
}
