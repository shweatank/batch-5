#include"header.h"
void main()
{
int fd;
fd=open("data",O_WRONLY|O_APPEND|O_CREAT,0664);
if(fd<0)
{
perror("open");
return;
}
printf("before\n");
struct flock v;
v.l_type=F_WRLCK;
v.l_whence=SEEK_SET;
v.l_start=0;
v.l_len=0;
fcntl(fd,F_SETLKW,&v);
printf("after\n");
for(char ch='A';ch<='E';ch++)
{
write(fd,&ch,1);
sleep(1);
}
v.l_type=F_UNLCK;
fcntl(fd,F_SETLK,&v);
printf(" capitals done\n");
}
