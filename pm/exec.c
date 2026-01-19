#include"header.h"
void main()
{
if(fork()==0)
execl("/bin/ls","ls",NULL);
else
{
if(fork()==0)
{
char*argv[]={"/bin/ls",NULL};
execv(argv[0],argv);
}
else
{
if(fork()==0)
execlp("pwd","pwd",NULL);
else
{
char*argv[]={"pwd",NULL};
execvp(argv[0],argv);
}
}
}


}
