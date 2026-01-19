#include"header.h"
void main()
{
int fd,a[5];
close(0);
fd=open("data4",O_RDONLY);
if(fd<0)
{
perror("open");
return;
}
int i;
for(i=0;i<5;i++)
scanf("%d",&a[i]);
for(i=0;i<5;i++)
printf("%d ",a[i]);

}
