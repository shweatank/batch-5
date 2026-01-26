#include"header.h"
void main()
{
int fd;
fd=open("/dev/char_driver",O_RDWR);
if(fd<0)
{
perror("open");
exit(0);
}
char s[20]="jugal kishore";
write(fd,s,strlen(s)+1);
/*close(fd);
fd=open("/dev/char_driver",O_RDWR);
if(fd<0)
{
perror("open");
exit(0);
}*/
char d[20];
read(fd,d,sizeof(d));
printf("%s\n",d);
close(fd);
}

