#include"header.h"

void main(int argc, char **argv)
{
int sfd,nsfd;
struct sockaddr_in sid,clid;
int len;
//socket
if(argc<3)
{
printf("usage: ./s1 port ip\n");
return;
}

sfd=socket(AF_INET,SOCK_STREAM,0);
if(sfd<0)
{
perror("socket");
return;
}
sid.sin_family=AF_INET;
sid.sin_port=htons(atoi(argv[1]));
sid.sin_addr.s_addr=inet_addr(argv[2]);

len=sizeof(sid);
//bind
if(bind(sfd,( struct sockaddr *)&sid,len)<0)
{
perror("bind");
return;
}

//listen
if(listen(sfd,5)<0)
{
perror("listen");
return;
}
printf("waiting for client\n");
//accept
nsfd=accept(sfd, (struct sockaddr *)&clid,&len);
if(nsfd<0)
{
perror("nsfd");
return;
}
int num,r;
read(nsfd,&num,4);
if(num<0)
{
r=0;
write(nsfd,&r,4);
}
else
{
r=1;
write(nsfd,&r,4);
}
}
