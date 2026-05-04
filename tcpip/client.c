#include"header.h"

void main(int argc, char **argv)
{
int sfd,nsfd;
struct sockaddr_in sid,clid;
int len;
//socket
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
if(connect(sfd,( struct sockaddr *)&sid,len)<0)
{
perror("bind");
return;
}
char s[20];
printf("enter the string\n");
scanf("%s",s);

write(sfd,s,strlen(s)+1);

int r;
read(sfd,&r,4);
if(r==1)
printf("palindrome\n");
else
printf("Not palindrome\n");

}


