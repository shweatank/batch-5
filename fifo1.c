#include"header.h"
void main()
{       
        int r=mkfifo("f1",0664);
        perror("mkfifo");
        int fd=open("f1",O_RDONLY);
        char s[10];
        read(fd,s,sizeof(s));
	printf("%s\n",s);
}       
           
