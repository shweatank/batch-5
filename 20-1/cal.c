#include"header.h"
void main()
{
int fd=open("/dev/basic_char",O_RDWR|O_TRUNC);
if(fd<0)
{
perror("open");
return;
}
char s[20];
printf("enter format(num1 op num2):\n");
scanf("%s",s);
write(fd,s,strlen(s)+1);
close(fd);
fd=open("/dev/basic_char",O_RDWR|O_TRUNC);
if(fd<0)
{
perror("open");
return;
}
int res;
read(fd,&res,sizeof(int));
printf("result=%d\n",res);
close(fd);
}
