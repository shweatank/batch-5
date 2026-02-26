#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<stdlib.h>

struct calc_req
{
	int n1;
	int n2;
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
	struct calc_req rand;
	rand.n1=10;
	rand.n2=20;

	int fd=open("/dev/calc_ioctl",O_RDWR);
	if(fd<0)
	{
		perror("open");
		exit(0);
	}
	
	ioctl(fd,CALC_IOC_ADD,&rand);
	printf("result :%ld\n",rand.result);
}

