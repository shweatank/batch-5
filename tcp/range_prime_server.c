#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>


int isprime(int num)
{
        int cnt=0,i;
        for(i=1;i<=num;i++)
        {
                if(num%i==0)
                {
                        cnt++;
                }
        }
        if(cnt==2)
        {
                return 1;
        }

        return 0;
}

int main(int argc,char **argv)
{
        if(argc!=2)
        {
                perror("argc");
                exit(0);
        }
        int sfd,cfd,num;
        int arr[10],primes[20],i,cnt;
        struct sockaddr_in saddr,caddr;
        puts("creating master socket.......");
        sfd=socket(AF_INET,SOCK_STREAM,0);    //family(ipv4)..type(tcp)...protocol
        if(sfd<0)
        {
                perror("sfd");
                return 0;
        }
        puts("master socket created...");
        saddr.sin_family=AF_INET;
        saddr.sin_port=htons(atoi(argv[1])); //LE to BE
        saddr.sin_addr.s_addr=inet_addr("0.0.0.0");

        if(bind(sfd,(const struct sockaddr *)&saddr,sizeof(saddr))<0)
        {
                perror("bind");
                exit(0);
        }
        puts("assign local address to the socket...");
        if(listen(sfd,1)<0)
        {
                perror("listen");
                return 0;
        }
        puts("waiting for client connections....");
        int csize=sizeof(caddr);
        cfd=(accept(sfd,(struct sockaddr*)&caddr,&csize));
        if(cfd<0)
        {
                perror("accept");
                return 0;
        }
        printf("server accept client connection.....");
        printf("-----client information-----\n");
        printf("client ip address:%s\n",inet_ntoa(caddr.sin_addr));
        printf("ip port no:%hu\n",ntohs(caddr.sin_port));

while(1)
{
        recv(cfd,arr,sizeof(arr),0);
        printf("recv from client");
        for (i = 0; i < 10; i++)
        {
            printf("%d ",arr[i]);
        }
        printf("\n");
        cnt=0;
        for (i = 0; i < 10; i++)
        {
            if(isprime(arr[i]))
            {
                primes[cnt++] = arr[i];
            }
        }
        send(cfd,primes,cnt* sizeof(int),0);
        printf("send to client prime numbers cnt:%d\n",cnt);
}
        close(sfd);
        close(cfd);

}


