#include"header.h"
int main()
{
	char s[20];
	mkfifo("f1",0664);
	perror("mkfifo");
	mkfifo("f2",0664);
	perror("mkfifo");
	int fd1=open("f1",O_RDWR);
	int fd2=open("f2",O_RDWR);
	if(fork()==0)
	{
          while(1)
         {
           read(fd1,s,sizeof(s));
           printf("%s",s);      
         }
	}
	else
	{
          while(1)
         {
          printf("enter the data\n");
          scanf("%s",s); 
           write(fd2,s,strlen(s)+1);
         }  
	}
}    

     
