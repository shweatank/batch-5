#include"header.h"
int main()
{
  int fd1,fd2;
  char s[20];
  mkfifo("f3",0666);
  mkfifo("f4",0666);
  perror("mkfifo");
  fd1=open("f3",O_RDWR);
  fd2=open("f4",O_RDWR);
 if(fork()==0)
 {
    read(fd1,s,sizeof(s));
    printf("%s\n",s);
 }
 else
 {
    printf("enter the data p2\n");
    scanf("%s",s);
    write(fd2,s,strlen(s)+1);           
}
}                    
  
