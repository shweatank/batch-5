#include"header.h"
void main()
{
int fd,ret;
char s[20]="jugal kishore";
fd=open("data2",O_WRONLY|O_TRUNC|O_CREAT,0664);
//fd=creat("data2",0664);
if(fd<0)
{
perror("open");
return;
}
ret=write(fd,s,sizeof(s));
if(ret<0)
{
perror("write");
return;
}
printf("%d\n",ret);
close(fd);
}
