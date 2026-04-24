#include"header.h"
void main()
{
int fd=open("/dev/rpi_uart",O_RDWR);
if(fd<0)
{
perror("open");
exit(0);
}
char tx[100]="jugal kishore";
char rx[100];

write(fd,tx,strlen(tx)+1);
sleep(5);
int n=read(fd,rx,sizeof(rx));
rx[n]='\0';
printf("%s\n",rx);
close(fd);
}
