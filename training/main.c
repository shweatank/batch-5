#include"header.h"
int p1[2],p2[2],p3[3],p4[4];
int n1=10,n2=20,result;
void SIGUSR1_ISR(int n)
{
if(n==SIGUSR1)
{
	read(p1[0],&result,sizeof(int));
	printf("add=%d\n",result);
}
else if(n==SIGUSR2)
{
	read(p2[0],&result,sizeof(int));
	printf("sub=%d\n",result);

}
else if(n==SIGSEGV)
{
	read(p3[0],&result,sizeof(int));
	printf("mul=%d\n",result);
}
else if(n==SIGFPE)
{
	read(p4[0],&result,sizeof(int));
	printf("div=%d\n",result);
}
}
void main()
{
	pipe(p1);
	pipe(p2);
	pipe(p3);
        pipe(p4);
	if(fork()==0)
	{
	
			result=add(n1,n2);
			write(p1[1],&result,sizeof(int));
			kill(getppid(),SIGUSR1);
		
	}
	else
	{
		if(fork()==0)
		{
			sleep(1);
			result=sub(n1,n2);
			write(p2[1],&result,sizeof(int));
			kill(getppid(),SIGUSR2);
		}
		else
		{
			if(fork()==0)
			{
				sleep(2);
				result=mul(n1,n2);
				write(p3[1],&result,sizeof(int));
				kill(getppid(),SIGSEGV);
			}
			else
			{
				if(fork()==0)
				{
					sleep(3);
					result=mul(n1,n2);
					write(p4[1],&result,sizeof(int));
					kill(getppid(),SIGFPE);
				}
				else
				{
					struct sigaction v1,v2,v3;
					v1.sa_handler=SIGUSR1_ISR;
					v1.sa_flags=SA_NODEFER;
					sigemptyset(&v1.sa_mask);
					sigaction(SIGUSR1,&v1,0);
					sigaction(SIGUSR2,&v1,0);		
					sigaction(SIGSEGV,&v1,0);
					sigaction(SIGFPE,&v1,0);
					while(1)
						pause();
				}
			}
	}
}
}
