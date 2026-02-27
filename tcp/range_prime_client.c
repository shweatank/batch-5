#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(int argc,char **argv)
{
        if(argc!=3)
        {
                perror("argc");
                exit(0);
        }
        int cfd,min,max,arr[10];
        int i;
        int primes[10];
        int bytes;
        struct sockaddr_in saddr,caddr;
        puts("client socket creating....\n");
        cfd=socket(AF_INET,SOCK_STREAM,0);
        if(cfd<0)
        {
                perror("sfd");
                return 0;
        }
        printf("client socket created...\n");
        saddr.sin_family=AF_INET;
        saddr.sin_port=htons(atoi(argv[1]));
        saddr.sin_addr.s_addr=inet_addr("0.0.0.0");

        if(connect(cfd,(const struct sockaddr *)&saddr,sizeof(saddr))<0)
        {
                perror("connect");
                return 0;
        }
        printf("connected with server\n");
        char result[100]={0};
        srand(getpid());
        while(1)
        {
              puts("enter min and max range:");
              scanf("%d%d",&min,&max);
              for(i=0;i<=10;i++)
              {
                      arr[i]=(rand()%(max-min+1)+min);
              }
              for(i=0;i<=10;i++)
              {
                      printf("%d ",arr[i]);
              }
              printf("\n");
              send(cfd,arr,sizeof(arr),0);
              puts("waiting for reply from server....\n");
              memset(primes, 0,sizeof(primes));
              bytes= recv(cfd,primes,sizeof(primes),0);
              if (bytes> 0)
              {
                 int cnt = bytes/sizeof(int);
                 printf("Prime numbers received from server: ");
                 for (i = 0; i < cnt; i++)
                 {
                          printf("%d ", primes[i]);
                 }
                 printf("\n");
              }
        }
        close(cfd);
}


