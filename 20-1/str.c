#include"header.h"
void main()
{
int fd=open("/dev/basic_char",O_RDWR|O_TRUNC);
char s[50];
printf("enter string:\n");
scanf("%[^\n]",s);
write(fd,s,strlen(s)+1);
printf("str=%s\n",s);
close(fd);
fd=open("/dev/basic_char",O_RDWR);
read(fd,s,strlen(s)+1);
printf("reverse=%s\n",s);
close(fd);
}
