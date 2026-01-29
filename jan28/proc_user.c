#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

struct data
{
	int n1,n2;
	char ch;
};

static int result ;
int main()
{
	struct data d;
	d.n1=10;
	d.n2=20;
	d.ch='M';
	int fd=open("/proc/proc_driver",O_RDWR);
	perror("open");
	if(fd<0)
	{
		perror("open");
	}
	write(fd,&d,sizeof(d));

//	read(fd,result,sizeof(result));
//	printf("res : %d\n",result);
	perror("write");
}
