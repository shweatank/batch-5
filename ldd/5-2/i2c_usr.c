#include"header.h"
struct info
{
char sa;
char mr;
char data;
};
void main()
{
int fd=open("/dev/i2c_file",O_RDWR);
if(fd<0)
{
perror("open");
exit(0);
}
struct info v;
v.sa=0xD0;
v.mr=0x01;
v.data='A';
write(fd,&v,sizeof(struct info));
read(fd,&v,sizeof(struct info));
printf("%c\n",v.data);
}
