#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main(){
	int i;
	pid_t p=fork();
	if(p<0)
		perror("fork");
	//if(p==0){
	while(1){
	/*for(i=0;i<10;i++){
	//	while(1);
		int k=system("./ex");
		//while(1);
		if(k<0)
			perror("system");
	}*/
	//}
			for(i=0;i<10;i++){
			int k=system("./ex1");
			if(k<0)
				perror("system");
		}
	}

}
