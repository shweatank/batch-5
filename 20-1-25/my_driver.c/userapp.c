#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

void main()
{
int fd;

fd=open("/dev/mydev",O_RDWR);
if(fd<0)
{
	perror("open");
	return;
}

int num;

printf("enter the num\n");
scanf("%d",&num);

write(fd,&num,sizeof(num));

close(fd);




}
