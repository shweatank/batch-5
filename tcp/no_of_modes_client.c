#include"header.h"

int main(int argc , char **argv)
{
        int fd,Csize;
        char str[100];
        struct sockaddr_in saddr,caddr;
        size_t len=0;

        puts("creating socket...");
        fd=socket(AF_INET,SOCK_STREAM,0);
        if(fd<0)
        {
                perror("socket");
                return 0;
        }
        puts("socket created ..!");


        saddr.sin_family=AF_INET;
        saddr.sin_port=htons(atoi(argv[1]));
        saddr.sin_addr.s_addr=inet_addr(argv[2]);

        if(connect(fd,(const struct sockaddr *)&saddr,sizeof(saddr))<0)
        {
                perror("connect");
                return 0;
        }
        puts("connected with server..!");


        while(1)

        {
        puts("enter string to send to the server...");
        fgets(str,100,stdin);
        str[strlen(str)-1]=0;


        send(fd,str,strlen(str)+1,0);

        puts("waiting for reply from seerver....");

        recv(fd,&len,sizeof(len),0);


        printf(" number of modes:%lu\n",len);
        }
        close(fd);


}


