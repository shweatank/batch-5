#include "headers.h"

void sendData(char *sdata,int sfd) {
	int status;
        if (!strcmp(sdata, "exit")) {
		send(sfd, sdata, strlen(sdata) + 1, 0);
		return;
	}
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
	if(argc != 2) {
		printf("Invalid Arguments..!");
		return 0;
	}
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd == -1) {
		puts("Socket error..!");
		return 0;
	}
	puts("Server socket successfully created");
	struct sockaddr_in saddr;
	int slen = sizeof(saddr);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(argv[1]));
	saddr.sin_addr.s_addr = INADDR_ANY;
	//saddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	
	if(bind(sfd,(const struct sockaddr *)&saddr,slen) == -1) {
		puts("bind error..!");
		return 0;
	}
	puts("Server bound to port_no and IP");

	if(listen(sfd,1) == -1) {
		puts("listen error..!");
		return 0;
	}
	puts("connection queue has created");

	puts("\nwaiting for 3-way HS to be initiated by client");
	//client controlling code
	int cfd;struct sockaddr_in caddr;
	int clen = sizeof(caddr);

	cfd = accept(sfd,(struct sockaddr *)&caddr,&clen);
	printf("Client successfully connected");
	printf("Client port number: %u\n",ntohs(caddr.sin_port));
	printf("Client IP address: %s\n",inet_ntoa(caddr.sin_addr));

	//data reading from client
	char *rdata;
	while(1) {
		puts("\nwaiting for data from client");
		rdata = recvData(cfd);
        	if (!strcmp(rdata, "exit")) {
			puts("Client has terminated the communication");
			break;
		}
		printf("%s",rdata);
		puts("\nSending the recieved data to client");
		sendData(rdata,cfd);
	}
	close(cfd);
	close(sfd);

}


	

