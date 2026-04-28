#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "10.255.73.36"  // Replace with your server's IP
int main(int argc,char **argv) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024];
    int bytes_received;

    // 1. Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // 2. Define server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return 1;
    }

    // 3. Connect to server
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }
    printf("Connected to server %s:%d\n", SERVER_IP, atoi(argv[1]));

    // 4. Send a message to server
    strcpy(buffer, "Hello from Raspberry Pi client!");
    send(sock, buffer, strlen(buffer), 0);

    // 5. Receive a response
      while(1)
      {
         bytes_received = recv(sock, buffer, sizeof(buffer)-1, 0);

   // sleep(1);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("Server: %s\n", buffer);
    }
      }

    // 6. Close socket
    close(sock);
    return 0;
}
