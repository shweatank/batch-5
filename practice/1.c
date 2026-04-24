#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
int fd;
fd=open("/dev/my_char",O_RDWR);
if(fd<0)
{
perror("open");
return 0;
}
close(fd);
return 0;
}
