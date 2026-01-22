#include"header.h"
int fd1,fd2;
void* thread1_write(void*p)
{
char s[20];
while(1)
{
printf("enter the string:\n");
scanf("%s",s);
write(fd1,s,9);
}
}
void* thread1_read(void*p)
{
char s[20];
while(1)
{
read(fd1,s,9);
printf("%s\n",s);
}
}
void main()
{
if(mkfifo("f1",0664)<0)
{
perror("mkfifo");
exit(0);
}
if(mkfifo("f2",0664)<0)
{
perror("mkfifo");
exit(0);
}
fd1=open("f1",O_RDWR);
if(fd1<0)
{
perror("open");
exit(0);
}
fd2=open("f2",O_RDWR);
if(fd2<0)
{
perror("open");
exit(0);
}
pthread_t t1,t2;
t1=pthread_create(&t1,0,thread1_write,0);
t2=pthread_create(&t2,0,thread1_read,0);
pthread_exit(0);
}
