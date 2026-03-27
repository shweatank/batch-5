#include"header.h"
int main()
{
  int fd1,fd2;
  char s[20];
  mkfifo("f3",0666);
  perror("mkfifo");
  mkfifo("f4",0666);
  perror("mkfifo");
  fd1=open("f3",O_RDWR);
  perror("open");
  fd2=open("f4",O_RDWR);
  perror("open");
 printf("enter the datain p1\n");
 scanf("%s",s);
 write(fd1,s,strlen(s)+1);           
 read(fd2,s,sizeof(s));
 printf("%s\n",s);
}                    
  
