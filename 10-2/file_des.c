#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
void main()
{
printf("pid=%d\n",getpid());
int fd=open("data",O_WRONLY|O_CREAT|O_TRUNC,0666);
while(1);
}
