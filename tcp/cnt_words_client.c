#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<string.h>
int main(int argc,char*argv[])
{
        int sfd,cnt=0;
        struct sockaddr_in saddr;
        puts("creatingsocket");
        sfd=socket(AF_INET,SOCK_STREAM,0);
        if(sfd<0)
        {
                perror("socket");
                exit(0);
        }
        puts("socket created");
        saddr.sin_family=AF_INET;
        saddr.sin_port=htons(atoi(argv[1]));
        saddr.sin_addr.s_addr=inet_addr("0.0.0.0");
        if(connect(sfd,(const struct sockaddr*)&saddr,sizeof(saddr))<0)
        {
                perror("connect");
                exit(0);
        }
        puts("client connected");
        puts("enter string");
        char str[100];
        fgets(str,100,stdin);
        str[strlen(str)-1]=0;
        send(sfd,str,strlen(str)+1,0);
        recv(sfd,&cnt,sizeof(cnt),0);
        printf("no fo words :%d",cnt);
        close(sfd);
}

