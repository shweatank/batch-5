#include"header.h"
void main()
{
pid_t pid;
printf("parent pid=%d\n",getpid());
if((pid=fork())==0)
{
execl("./cal","cal",NULL);
exit(0);
}
else
{
int status;
waitpid(pid,&status,0);
if(WIFEXITED(status))
printf("child exit status= %d\n",WEXITSTATUS(status));
}
}
