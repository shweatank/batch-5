#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>


int main()
{
    int fd;
    char buf[100];
    char ch='\0';
    int n;

    fd = open("/dev/rpi_uart", O_RDWR);

    if(fd < 0)
    {
        perror("open");
        return 0;
    }

    printf("UART Receiver Ready\n");
        n = read(fd, buf,sizeof(buf));
            printf("Received: %s\n", buf);
    close(fd);
    }
