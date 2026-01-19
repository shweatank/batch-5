#include"header.h"
int pid;
struct sigaction *v;
void SIGCHLD_ISR(int n)
{
int status;
waitpid(pid,&status,WNOHANG|WUNTRACED|WCONTINUED);
if(WIFEXITED(status))
printf("exit normally %d\n",WEXITSTATUS(status));
else if(WIFSTOPPED(status))
printf("susoended signal no. %d\n",WSTOPSIG(status));
else if(WIFCONTINUED(status))
printf("contined with 18\n");
else if(WIFSIGNALED(status))
printf("signaled with %d\n",WTERMSIG(status));
}
void main()
{
if((pid=fork())==0)
{
printf("chld=%d par=%d\n",getpid(),getppid());
sleep(10);
exit(0);
}
else
{
v=calloc(1,sizeof(struct sigaction));
v->sa_handler=SIGCHLD_ISR;
sigemptyset(&v->sa_mask);
v->sa_flags=0;
sigaction(SIGCHLD,v,0);
printf("parent is waiting for SIGCHLD signal %d\n",getpid());
while(1);
}
}
