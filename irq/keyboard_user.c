#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

struct oper
{
	int n1;
	int n2;
	char op;
};

int main()
{
	struct oper;
	oper.n1=10;
	oper.n2=20;
	oper.op='+';
	int fd=open("/dev/irq_demo_driver",O_RDWR);
	if(fd<0)
	{
		perror("open");
		return 0;
	}
	write(fd,&oper,sizeof(*oper));
	close(fd);
}
