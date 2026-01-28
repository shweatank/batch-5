#include"header.h"


struct calc
{
int a;
int b;
int res;
}v;


void main()
{
int fd=open("/proc/proc_basic",O_RDWR);
if(fd<0)
{
perror("open");
exit(0);
}
v.a=10;
v.b=20;
write(fd,&v,sizeof(struct calc));
read(fd,&v,sizeof(struct calc));
printf("%d\n",v.res);
close(fd);
}
