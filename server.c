#include"header.h"

void main(int argc, char ** argv)
{
int sfd;

struct sockaddr_in sid,cid;

sfd=socket(AF_INET,SOCK_DGRAM,0);
if(sfd<0)
{
perror("socket");
return;
}
perror("socket");

sid.sin_family=AF_INET;
sid.sin_port=htons(atoi(argv[1]));
sid.sin_addr.s_addr=inet_addr(argv[2]);

int len=sizeof(sid);

if(bind(sfd,(struct sockaddr *)&sid,len)<0)
{
perror("bind");
return;
}
/*
if(listen(sfd,5)<0)
{
perror("listen");
return;
}
*/
char s[20];
recvfrom(sfd,s,sizeof(s),0,(struct sockaddr *)&cid,&len);

printf("received from client:%s\n",s);

char s1[20]="hello";
sendto(sfd,s1,strlen(s1)+1,0,(struct sockaddr *)&cid,len);
}


