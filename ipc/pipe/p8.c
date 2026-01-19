#include"header.h"
char s[10];
int p[2];
void ISR(int n)
{
wait(0);
read(p[0],s,sizeof(s));
printf("after converted string read by parent =%s\n",s);
exit(0);
}
void main()
{
pipe(p);
if(fork()==0)
{
  read(p[0],s,sizeof(s));
printf("child readed before converting=%s\n",s);
  int i;
 for(i=0;s[i];i++)
 if(s[i]>='a'&&s[i]<='z')
 s[i]-=32;
write(p[1],s,strlen(s)+1);
printf("child converted =%s\n",s);
exit(0);
}
else
{
   printf("in par enter string:");
  scanf("%[^\n]",s);
 write(p[1],s,strlen(s)+1);
struct sigaction v;
v.sa_handler=ISR;
v.sa_flags=0;
sigemptyset(&v.sa_mask);
sigaction(SIGCHLD,&v,0);
while(1);
}
}
