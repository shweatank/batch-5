#include"header.h"


struct calc
{
int a;
int b;
int res;
char s[20];
}v;

#define MAGIC 'P'

#define ADD _IOWR(MAGIC,1,struct calc)

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

strcpy(v.s,"mul");
write(fd,&v,sizeof(struct calc));
read(fd,&v,sizeof(struct calc));
printf("%d\n",v.res);


}
