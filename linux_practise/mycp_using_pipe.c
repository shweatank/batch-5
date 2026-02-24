#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc,char **argv) {
	if(argc != 3) {
		puts("Invalid Argument..!");
		return 0;
	}
	int pipefd[2];
	int srcfd = open(argv[1],O_RDONLY);
	if(srcfd == -1) {
		puts("Unable to open source file..!");
		return 0;
	}
	int destfd = open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0644);
	if(destfd == -1) {
		puts("Unable to open/create destination file..!");
		return 0;
	}
	int p = pipe(pipefd);
	if(p == -1) {
		puts("pipe error");
		return 0;
	}
	char ch;
	while(read(srcfd,&ch,1) == 1) {
		write(pipefd[1],&ch,1);
	}
	close(pipefd[1]);
	while(read(pipefd[0],&ch,1) == 1) {
		write(destfd,&ch,1);
	}
	close(pipefd[0]);
}
