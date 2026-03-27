#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>
void isr()
{
	printf("main received:\n");


}

void isr2()
{
	printf("main received from child 2\n");
}
void isr3()
{
	printf("main recevied from child 3\n");
}

void isr4()
{
	printf("main recevied from child 4\n");
}
void main()
{

	int sd=shmget(99,1024,IPC_CREAT|0664);
	perror("shmget");

	//	pause();

	if(fork()==0)
	{



	//	printf("%d\n",getppid());

		int a=10,b=20;
		int result=a+b;

		int  *p=(int *)shmat(sd,NULL,0);

		*p=result;

		shmdt(p);
		perror("shmdt");
		kill(getppid(),SIGUSR2);
		// exit(0);




	}
	else
	{




		pid_t child2=fork();
		if(child2==0)
		{


			signal(SIGUSR1,isr2);

			pause();

			int a=10,b=20;
			int result=a-b;

			int  *p=(int *)shmat(sd,NULL,0);

			*p=result;

			shmdt(p);
			perror("shmdt");

			kill(getppid(),SIGUSR1);

			// exit(0);

		}
		else
		{
			 pid_t child3=fork();
			if(child3==0)
			{
                               signal(SIGUSR1,isr);
			       pause();

			       int a=10,b=20;
			       int result=a*b;

		               int  *p=(int *)shmat(sd,NULL,0);
			       *p=result;

			       kill(getppid(),SIGUSR2);


			}
			else
			{

				pid_t child4=fork();

				if(child4==0)
				{
					signal(SIGUSR1,isr);
					pause();

					int a=10,b=20;

					int result=b/a;

					
			               int  *p=(int *)shmat(sd,NULL,0);

				       *p=result;

				       kill(getppid(),SIGUSR2);

				}
				else
				{		  

					signal(SIGUSR2,isr);
					pause();

					printf("%d\n",getpid());

					int *p=(int*)shmat(sd,NULL,0);

					printf("result of addition =%d\n",*p);

					kill(child2,SIGUSR1);

					signal(SIGUSR1,isr2);
					pause();

					int *s=(int *)shmat(sd,NULL,0);

					printf("resut of subraction=%d\n",*s);

					kill(child3,SIGUSR1);
					pause();

					signal(SIGUSR2,isr3);

					int *d=(int*)shmat(sd,NULL,0);

					printf("multiplication result=%d\n",*d);

                                    
					kill(child4,SIGUSR1);
					pause();

					signal(SIGUSR2,isr4);

					int *m=(int *)shmat(sd,NULL,0);

					printf(" division result=%d\n",*m);



					

				}
			}

		}
	}
}
