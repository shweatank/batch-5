#include"header.h"
void main()
{
int fd=open("/dev/rpi_uart",O_RDWR|O_NONBLOCK);
if(fd<0)
{
perror("open");
exit(0);
}
char tx[100]="abhishek";
char rx[100];

write(fd,tx,strlen(tx)+1);
int n=read(fd,rx,sizeof(rx));
if(n>0)
{
rx[n]='\0';
printf("%s\n",rx);
}
else
printf("no data\n");
close(fd);
}
