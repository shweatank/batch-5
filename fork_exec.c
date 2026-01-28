#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
void run_other_function()
{
	printf("Inside 'run_other_function' via exec\n");
}

int main(int argc,char **argv)
{
	if(argc==2 && strcmp(argv[1],"run_function")==0)
	{
		run_other_function();
		return 0;
	}

	printf("Parent starting:\n");

	pid_t pid=fork();
	if(pid==-1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if(pid==0)
	{
		printf("In child... now calling exec\n");
	
		char *args[]={argv[0],"run_function",NULL};  // This means ./a.out run_function

		if(execv(argv[0],args)==-1)
		{
			perror("Exec failed");
			exit(EXIT_FAILURE);
		}

	}
	else
	{
		printf("In parent.... Parent is waiting for the child to terminate\n");
		wait(NULL);
		printf("Child has finished execution\n");
	}
	return 0;
}
