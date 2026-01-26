#include"header.h"
void main()
{
int fd;
fd=open("/dev/char_file",O_RDWR);
if(fd<0)
{
perror("open");
exit(0);
}
close(fd);
}

