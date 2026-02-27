#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(int argc,char **argv)
{
        int cfd,Csize;
        char str[100];
        struct sockaddr_in saddr;
        size_t len;
        if(argc!=3)
        {
                perror("arg");
                return 0;
        }

        puts("client socket creating");
        cfd=socket(AF_INET,SOCK_STREAM,0);
        if(cfd<0)
        {
                perror("cfd");
                return 0;
        }
        puts("created socket creating");

        saddr.sin_family=AF_INET;
        saddr.sin_port=htons(atoi(argv[1]));
        saddr.sin_addr.s_addr=inet_addr(argv[2]);
        if(connect(cfd,(const struct sockaddr*)&saddr,sizeof(saddr))<0)
        {
                perror("connect");
                return 0;
        }
        puts("connect to the server...");

        printf("enter the str");
        fgets(str,100,stdin);
        str[strlen(str)-1]='\0';

        send(cfd,str,sizeof(str),0);
        puts("waiting for the server reply");

        recv(cfd,&len,sizeof(str),0);
        puts("receive from the server");
        printf("%ld",len);

        close(cfd);

}

