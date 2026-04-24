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
struct info v,v1;
v.sa=0xD0;
v.mr=0x01;
v.data='A';
write(fd,&v,sizeof(struct info));
read(fd,&v1,sizeof(struct info));
printf("%x %d %c\n",v1.sa,v1.mr,v1.data);
}
