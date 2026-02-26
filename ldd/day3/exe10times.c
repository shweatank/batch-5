#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
void main()
{
	int i;
	int fd=open("/dev/ ",O_RDWR|O_CREAT|O_TRUNC,0664);
	for (i = 1; i <= 10; i++)
        write(fd,&i,sizeof(i));
        read(fd,&i,sizeof(i));
        for(i=1;i<=10;i++)
        printf("%d\n",i);
}

