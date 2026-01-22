#include"header.h"
void main()
{
int fd;
fd=open("/dev/memdemo",O_RDWR);
if(fd<0)
{
perror("open");
return;
}
int *p;
p=malloc(sizeof(int)*10);
printf("%d\n",p);
}
