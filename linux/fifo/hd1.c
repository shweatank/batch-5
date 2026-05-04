#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
char r[10];
char w[10];

mkfifo("f1",0664);
int fd1=open("f1",O_RDWR);
if(fd1<0)
{
perror("open");
return;
}
printf("enter writing\n");
scanf("%s",w);
write(fd1,w,strlen(w)+1);
