// rx_app.c
#include <stdio.h>
#include <fcntl.h>
#include<string.h>
#include <unistd.h>

int main()
{
    int fd = open("/dev/rpi_uart", O_RDWR);
    char str[]="sai kumar";
    char rx[100];
    write(fd,str,strlen(str)+1);
    sleep(6);
    int n=read(fd,rx,sizeof(rx));
    rx[n]='\0';
    printf("%s\n",rx);
    close(fd);
    return 0;
}
