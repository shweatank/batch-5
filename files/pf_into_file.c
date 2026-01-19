#include"header.h"
void main()
{
int fd,ret;
close(1);
//fd=open("data3",O_WRONLY|O_TRUNC|O_CREAT,0664);
fd=creat("data3",0644);
if(fd<0)
{
perror("creat");
return;
}
printf("sai kumar fd=%d\n",fd);
}
