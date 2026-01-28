#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>


int main(){
	int ret;
	//for(int i=0;i<5;i++){
	while(1){
	/*	ret=fork();
		if(ret==0){
		//	printf("%d\n",i);
			execl("file_exe","file_exe",NULL);
		}*/

		//system("/home/dell/practice/20_01_2026/file_exe");
		system("./file_exe");
	}
}

