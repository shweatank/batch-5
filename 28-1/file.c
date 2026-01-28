#include"header.h"
void main()
{
int fd=open("/dev/my_file",O_RDWR);
if(fd<0)
{
perror("open");
exit(0);
}
int res;
read(fd,&res,sizeof(int));
printf("%d\n",res);
//close(fd);
}
