#include"header.h"
void main(int argc,char**argv)
{
if(argc<3)
{
printf("usage:kill -signalno pid\n");
return;
}
int pid,sig_no,i;
sig_no=atoi(argv[1]+1);
for(i=2;i<argc;i++)
{
pid=atoi(argv[i]);
kill(pid,sig_no);
}
}
