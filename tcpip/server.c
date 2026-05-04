#include"header.h"

int my_strcmp(const char *p, const char *s);
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
char s[20];
read(nsfd,s,sizeof(s));
printf("received string=%s\n",s);
char p[20];
strcpy(p,s);
int slen=strlen(s);
int i,j=slen-1;

for(i=0;i<j;i++,j--)
{
char t;
t=s[i];
s[i]=s[j];
s[j]=t;
}
printf("reversed string=%s\n",s);
int r=my_strcmp(p,s);
write(nsfd,&r,4);
}

int my_strcmp(const char *p, const char *s)
{
int i;
for(i=0;s[i];i++)
{
if(s[i]!=p[i])
break;
}
if(s[i]==p[i])
return 1;
else 
return 0;
}



