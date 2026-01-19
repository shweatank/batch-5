#include"header.h"
void main()
{
int fd,ret;
fd=open("data",O_RDONLY);
if(fd<0)
{
perror("open");
return;
}
printf("%d\n",fd);



}
