#include"header.h"
void main()
{
int fd,ret;
int a[5];
//bzero(a,sizeof(a));
memset(a,0,sizeof(a));
fd=open("data1",O_RDONLY);
if(fd<0)
{
perror("open");
return;
}
for(int i=0;i<5;i++)
printf("%d ",a[i]);
printf("\n");
ret=read(fd,a,sizeof(a));
for(int i=0;i<5;i++)
printf("%d ",a[i]);
printf("\n");

}
