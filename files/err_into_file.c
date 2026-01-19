#include"header.h"
void main()
{
int fd1;
close(2);
fd1=creat("data4",0644);
if(fd1<0)
{
perror("open");
return;
}
int *p=100;
printf("%d\n",*p);
}
