#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
	char str[100];
	while(1) {
		printf("Enter the string to send using pipe: ");
		scanf("%s",str);
		if(strcmp(str,"exit") == 0) break;
		int sendPiped[2],recvPiped[2];
		if(pipe(sendPiped) == -1 || pipe(recvPiped) == -1) {
			perror("pipe");
			return 0;
		}
		if(fork() == 0) {
			char s[100],res[100];
			int rescnt = 0;
			close(sendPiped[1]);
			read(sendPiped[0],s,100);
			for(int i = 0;i<strlen(s);i++) {
				int count = 0;
				for(int j = 0;j<strlen(s);j++) {
					if(s[i] == s[j]) count++;
				}
				if(count != 1) {
					res[rescnt++] = s[i];
				}
			}
			res[rescnt] = '\0';
			close(recvPiped[0]);
			write(recvPiped[1],res,strlen(res)+1);
			exit(0);
		}
		else {
			close(sendPiped[0]);
			close(recvPiped[1]);
			write(sendPiped[1],str,strlen(str)+1);
			wait(NULL);
			read(recvPiped[0],str,100);
			printf("%s\n",str);
		}
	}
}


