#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
int main()
{
	int fd1,fd2,fd3,fd4;
	fd1=open("sai.txt",O_CREAT|O_RDONLY,0666);
	fd2=open("sai.txt",O_CREAT|O_RDONLY,0666);
	fd3=open("sai.txt",O_CREAT|O_RDONLY,0666);
	fd4=open("sai.txt",O_CREAT|O_RDONLY,0666);
	sleep(30);

}
