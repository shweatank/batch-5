#include"header.h"
void main()
{
int fd,ret;
fd=open("data2",O_RDONLY);
if(fd<0)
{
perror("open");
return;
}
char s[30];
ret=read(fd,s,20);
if(ret<0)
{
perror("read");
return;
}
s[ret]='\0';
printf("%s\n",s);
printf("ret=%d fd=%d\n",ret,fd);



}
