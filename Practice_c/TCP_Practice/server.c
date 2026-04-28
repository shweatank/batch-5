#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include <netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
int main(int argc,char **argv)
{
	if(argc!=2)
	{
		printf("try again with port number:");
		return 0;
	}
	struct sockaddr_in saddr,caddr;
	socklen_t Ssize,Csize;
	int sfd,cfd,len;
	char str[30];
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd==-1)
	{
		perror("socket");
		return 0;
	}
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(atoi(argv[1]));
	saddr.sin_addr.s_addr=inet_addr("0.0.0.0");
	Ssize=sizeof(saddr);
	if(bind(sfd,(struct sockaddr*)&caddr,&Csize)==-1)
	{
		perror("bind");
		return 0;
	}

	if(listen(sfd,1)==-1)
	{
		perror("listen");
		return 0;
	}
	Csize=sizeof(caddr);
	cfd=accept(sfd,(const struct sockaddr*)&caddr,&Csize);
	if(cfd==-1)
	{
		perror("accept");
		return 0;
	}
	int s=recv(cfd,str,sizeof(str),0);
	if(s<=0)
		perror("recv");
	send(cfd,str,strlen(str),0);
	close(sfd);
	close(cfd);
}
