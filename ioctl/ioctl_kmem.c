#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<unistd.h>

struct clac_req
{
	int a;
	int b;
	long result;
	int err;
};

#define CALC_IOC_MAGIC 'C'

#define CALC_IOC_ADD  _IOWR(CALC_IOC_MAGIC, 1, struct calc_req)
#define CALC_IOC_SUB  _IOWR(CALC_IOC_MAGIC, 2, struct calc_req)
#define CALC_IOC_MUL  _IOWR(CALC_IOC_MAGIC, 3, struct calc_req)
#define CALC_IOC_DIV  _IOWR(CALC_IOC_MAGIC, 4, struct calc_req)
#define CALC_IOC_MOD  _IOWR(CALC_IOC_MAGIC, 5, struct calc_req)


int main()
{
	struct calc_req *rand;
	int fd1=open("/dev/calc_ioctl",O_RDWR);
	if(fd1<0)
	{
		perror("open");
		exit(0);
	}
	int fd2=open("/dev/memdemo",O_RDWR);
	if(fd2<0)
	{
		perror("open");
		exit(0);
	}

	kmalloc(sizeof(*rand),
	
}
