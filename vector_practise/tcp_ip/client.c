#include "headers.h"

void sendData(char *sdata,int sfd) {
        int status;
        status = send(sfd, sdata, strlen(sdata) + 1, 0);
        if (status <= 0) return;
}

char* recvData(int cfd) {
        static char rdata[100];
        int status;
        status = recv(cfd, rdata, 100, 0);
        if (status == 0) {
                puts("client closed connection");
                return NULL;
        }
        if (status < 0) {
                perror("recv");
                return NULL;
        }
        return rdata;
}

int main(int argc,char **argv) {
	if(argc != 3) {
		puts("Invalid Arguments..!");
		return 0;
	}

	int sfd;
	if((sfd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
		puts("Socket error..!");
		return 0;
	}

	struct sockaddr_in saddr;
	int slen = sizeof(saddr);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(argv[2]));
	saddr.sin_addr.s_addr = inet_addr(argv[1]);

	if(connect(sfd,(const struct sockaddr*)&saddr,slen) == -1) {
		puts("Client connect error..!");
		return 0;
	}
	puts("Client connected..");

	//sending data to server
	char sdata[100];
	char *rdata;
	while(1) {
		printf("Enter the data to send to server: ");
		scanf("%99s", sdata);
		sendData(sdata,sfd);
        	if (!strcmp(sdata, "exit")) break;
		puts("Receiving the data from server: ");
		rdata = recvData(sfd);
		printf("%s\n",rdata);
	}

	close(sfd);
}

