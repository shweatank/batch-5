#include<stdio.h>
#include<sys/socket.h>

int main(int argc,char **argv)
{
	if(argc!=3)
	{
		printf("try again\n");
		return 0;
	}
	struct socketaddr *saddr,*caddr;
	int sfd,cfd;
	socket_len Ssize,Csize;
	char str[30];
	caddr.sin_family=AF_INET;
	caddr.sin_port=htons(atoi(argv[1]));
	caddr.sin_addr.s_addr=inet(argv[2]);
	sfd=socket(AF_INET,SOCKET_STREAM,0);
	if(sfd==-1)
	{
		perror("socket");
		return 0;
	}
	if(connect(sfd,(struct sockaddr *)&caddr,sizeof(caddr)))
	{
		perror("socket");
		return 0;
	}
	puts("enter the string to send:");
	fgets(str,29,stdin);
	if(send(sfd,str,strlen(str)+1,0)<=0)
	{
		perror("send");
		return 0;
	}

}
