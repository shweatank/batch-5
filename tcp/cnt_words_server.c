#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<string.h>
int main(int argc,char *argv[])
{
        int sfd;
        struct sockaddr_in saddr,caddr;
        puts("creating master socket");
        sfd=socket(AF_INET,SOCK_STREAM,0);
        if(sfd<0)
        {
                perror("socket");
                exit(0);
        }
        puts("master socket created");
        saddr.sin_family=AF_INET;
        saddr.sin_port=htons(atoi(argv[1]));
        saddr.sin_addr.s_addr=inet_addr("0.0.0.0");
       if(bind(sfd, (const struct sockaddr*)&saddr,sizeof(saddr))<0)
       {
               perror("bind");
               exit(0);
       }
       puts("assign localaddress to socket");
       puts("socket waitng for pending queue");
       if(listen(sfd,1)<0)
       {
               perror("listen");
               exit(0);
       }
       int csize=sizeof(caddr);
               int cfd;
       cfd=accept(sfd, (struct sockaddr *)&caddr,&csize);
       if(cfd<0)
       {
               perror("accept");
               exit(0);
       }
       puts("sever accepted client conection");
       puts("client info");
       printf("ip:%s\n",inet_ntoa(caddr.sin_addr));
       printf("port:%d\n",ntohs(caddr.sin_port));
       char str[100];
       recv(cfd,str,sizeof(str),0);
       char *ptr=str;
       int cnt=0;
       ptr=strtok(ptr," ");
       while(ptr!=NULL)
       {
               cnt++;
               ptr=strtok(NULL," ");
       }
       send(cfd,&cnt,sizeof(cnt),0);
       close(cfd);
       close(sfd);
}

