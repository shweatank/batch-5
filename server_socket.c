#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>

struct st
{
	int status;
};
struct st var={0},var2={0};

int main(int argc ,char **argv) {

	int server_fd, new_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char buffer[1024];

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == 0) {
		perror("Socket failed");
		exit(1);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(atoi(argv[1]));

	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		perror("Bind failed");
		exit(1);
	}

	if (listen(server_fd, 3) < 0) {
		perror("Listen failed");
		exit(1);
	}
	printf("Server listening on port %d\n", atoi(argv[1]));

	new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
	if (new_socket < 0) {
		perror("Accept failed");
		exit(1);
	}


	int fd=open("/dev/key_board_file",O_RDWR);
	perror("open");

	write(fd,&var,sizeof(var));
	perror("write");

	//waiting for interrupt trigger through keyboard:
	//
	//

	printf("waiting for the to press CNTR+2 to send signal:\n");
	read(fd,&var2,sizeof(var2));
	perror("read");

	//printf("%d",var2.status);

	// int valread = read(new_socket, buffer, 1024);
	// buffer[valread] = '\0';
	//printf("Client: %s\n", buffer);

	// momotiring status from keyboard:
	char s[100]="now start the reading temperature from sensor";
	send(new_socket,s,strlen(s), 0);
	close(new_socket);
	close(server_fd);
	return 0;
}
