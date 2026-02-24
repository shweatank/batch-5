#include "headers.h"

int main(int argc,char **argv) {
	if(argc != 2) {
		printf("Invalid Arguments..!");
		return 0;
	}
	int sfd = socket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in saddr;
	int slen = sizeof(saddr);
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(argv[1]));
	saddr.sin_addr.s_addr = INADDR_ANY;
	//saddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	
	bind(sfd,(const struct sockaddr *)&saddr,slen);

	listen(sfd,1);


	//client controlling code
	int cfd;struct sockaddr_in caddr;
	int clen = sizeof(caddr);

	cfd = accept(sfd,(struct sockaddr *)&caddr,&clen);
	
	printf("Client port number: %u\n",ntohs(caddr.sin_port));
	printf("Client IP address: %s\n",inet_ntoa(caddr.sin_addr));

	close (sfd);
	close (cfd);
}


	

