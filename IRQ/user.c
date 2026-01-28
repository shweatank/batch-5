#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>


void main()
{
int fd=open("/dev/my_file",O_RDONLY);
int res;
read(fd,&res,sizeof(int));
perror("read");
printf("result is %d\n",res);
close(fd);




}

