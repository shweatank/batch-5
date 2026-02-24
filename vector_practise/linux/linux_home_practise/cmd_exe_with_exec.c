#include <stdio.h>
#include <unistd.h>
#include<string.h>

int main(int argc,char *argv[]) {
	if(argc < 2) {
		printf("Invalid Arguments..");
		return 0;
	}
#define argv1 argv[1]
	printf("In a.out process and pid = %d\n",getpid());
	//execlp(argv[1],argv[1],argv[2],argv[3],NULL);
	//execl("path",argv[1],argv[2],NULL);
	//execv("path of executable file",argv+1);
	//execve("path of executable file",argv+1,"environment_vars[]");
	execvp(argv[1],argv+1);
}
