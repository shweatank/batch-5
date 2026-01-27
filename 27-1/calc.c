#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<unistd.h>

// ---------- IOCTL interface ----------
#define CALC_IOC_MAGIC  'C'

struct calc_req {
    int a;
    int b;
    long result;   // long to reduce overflow risk vs int (still possible)
};


#define CALC_IOC_ADD  _IOWR(CALC_IOC_MAGIC, 1, struct calc_req)

struct calc_req v;
void main()
{
int fd=open("/dev/calc_ioctl",O_RDWR);;
if(fd<0)
{
perror("open");
return;
}
	v.a=20;
	v.b=10;
	char op;
			ioctl(fd,CALC_IOC_ADD,&v);
read(fd,&v.result,sizeof(int));
close(fd);
}
