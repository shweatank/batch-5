#include"header.h"
#define DEVICE_NAME "calc_ioctl"
#define CALC_IOC_MAGIC  'C'

struct calc_req {
    int a;
    int b;
    long result;   // long to reduce overflow risk vs int (still possible)
};
#define CALC_IOC_ADD  _IOWR(CALC_IOC_MAGIC, 1, struct calc_req)
void main()
{
int fd=open("/dev/calc_ioctl",O_RDWR);
if(fd<0)
{
perror("open");
return;
}
struct calc_req v;
v.a=10;
v.b=20;
ioctl(fd,CALC_IOC_ADD,&v);
sleep(30);
close(fd);
}
