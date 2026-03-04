#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void main()
{
	int fd=open("data",O_WRONLY|O_CREAT|O_TRUNC,0664);	

         int i=10;

	write(fd,&i,4);
        close(fd);


         fd=open("data",O_RDONLY);


	 int j;

	 read(fd,&j,4);

	 printf("%d\n",j);

	 close(fd);

}
