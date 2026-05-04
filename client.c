#include"header.h"

void main(int argc,char ** argv)
{
int sfd,nsfd;
struct sockaddr_in sid,cid;

sfd=socket(AF_INET,SOCK_DGRAM,0);
if(sfd<0)
{
perror("socket");
return;
}
sid.sin_family=AF_INET;
sid.sin_port=htons(atoi(argv[1]));
sid.sin_addr.s_addr=inet_addr(argv[2]);
int len=sizeof(sid);

char s[20];
printf("enter the string\n");
scanf("%s",s);
sendto(sfd,s,strlen(s)+1,0,(struct sockaddr *)&sid,len);

char s1[20];
recvfrom(sfd,s1,sizeof(s1),0,0,0);
printf("server reply:%s\n",s1);
}
