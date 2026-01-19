#include"header.h"
void main()
{
int n;
mkfifo("f1",0664);
perror("mkfifo");
int fd=open("f1",O_RDONLY);
perror("open");
while(1)
{
if(read(fd,&n,4)==0)
break;
printf("%d\n",n);
}
}
