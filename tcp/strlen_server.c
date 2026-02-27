#include"header.h"

size_t mystrlen(const char *p)
{
        size_t i=0;
        while(p[i++]);
        return i-1;

}
int main(int argc , char **argv)
{
        int cfd,sfd,Csize;
        char str[100];
        struct sockaddr_in saddr,caddr;
        size_t len;
        puts("creating master socket...");
        sfd=socket(AF_INET,SOCK_STREAM,0);
        if(sfd<0)
        {
                perror("socket");
                return 0;
        }
        puts("Master socket created ..!");

        puts("assigning local address to the socket....");

        saddr.sin_family=AF_INET;
        saddr.sin_port=htons(atoi(argv[1]));
        saddr.sin_addr.s_addr=inet_addr("0.0.0.0");
         if(bind(sfd,(const struct sockaddr *)&saddr,sizeof(saddr))<0)

         {
                perror("bind");

                return 0;
        }
        puts("assign local address to the socket.!");

        puts("creating queue for pending connections...");

        if(listen(sfd,1)<0)
        {
                perror("listen");
                return 0;
        }

        puts("created queue for pending connections.!");



        puts("waiting for client connections...");
        Csize=sizeof(caddr);
        cfd=accept(sfd,(struct sockaddr *)&caddr,&Csize);
        if(cfd<0)
        {
                perror("accept");
                return 0;
        }

        puts("server accepted client connection...!");

        puts("client info is....");
        printf("Client IP: %s\n",inet_ntoa(caddr.sin_addr));
       // printf("Client Port: %hu ( without convert )\n",caddr.sin_port);
        printf("Client Port: %hu\n",ntohs(caddr.sin_port));

        recv(cfd,str,sizeof(str),0);

        len=mystrlen(str);

        send(cfd,&len,sizeof(len),0);




        close(cfd);
        close(sfd);


}

