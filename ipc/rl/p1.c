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
for(char ch='a';ch<='z';ch++)
{
write(fd,&ch,1);
sleep(1);
}
printf("done\n");
}
