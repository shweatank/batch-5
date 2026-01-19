#include"header.h"
void main()
{
int n;
mkfifo("f1",0664);
perror("mkfifo");
int fd=open("f1",O_RDWR);
perror("open");
while(1)
{
printf("enter number..\n");
scanf("%d",&n);
write(fd,&n,4);
if(n==5)
break;
}
}
